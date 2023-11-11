#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define Max_stu_size 5000

sem_t sem_student,sem_coordinator;
pthread_mutex_t seatLock,queueLock,tutorFinishedQueueLock;

int complete=0;
int totalRequests=0;
int totalSessions=0;
int tutoringNow=0;

int student_num=0;
int tutor_num=0;
int help_num=0;
int chair_num=0;
int occupied_chair_num=0;


int newArrivedStudentQueue[Max_stu_size];
int tutorFinishedQueue[Max_stu_size];
int priorityQueue[Max_stu_size][2];
int student_priority[Max_stu_size];
int student_id[Max_stu_size];
int tutor_id[Max_stu_size];


void *student_thread(void *student_id);
void *tutor_thread(void *tutor_id);
void *coordinator_thread();

int main()
{

    int i;

    printf("------------------------------------------------------------------\n");
    printf("\t\t Seeking tutor\t\t\n");
    printf("------------------------------------------------------------------\n\n\n");

    printf("Enter number of students: ");
    scanf("%d",&student_num);
    printf("Enter number of tutors: ");
    scanf("%d",&tutor_num);
    printf("Enter number of chairs: ");
    scanf("%d",&chair_num);
    /*printf("Number of help time for \n");
    for( i=0; i<student_num; i++)
    {
        printf("student %d : ",i+1);
        scanf("%d",&help_num[i]);
    }*/
    printf("Maximum help: ");
    scanf("%d",&help_num);

    if(student_num> Max_stu_size || tutor_num> Max_stu_size)
    {
        printf("The student/tutor is exceed than Max student size");
        exit(-1);
    }

    for(i=0; i<student_num; i++)
    {
        newArrivedStudentQueue[i]=-1;
        tutorFinishedQueue[i]=-1;
        priorityQueue[i][0]=-1;
        priorityQueue[i][1]=-1;
        student_priority[i]=0;
    }

    //initial sem value and mutex value
    sem_init(&sem_student,0,0);
    sem_init(&sem_coordinator,0,0);
    pthread_mutex_init(&seatLock,NULL);
    pthread_mutex_init(&queueLock,NULL);
    pthread_mutex_init(&tutorFinishedQueueLock,NULL);

    //create thread
    pthread_t students[student_num];
    pthread_t tutors[tutor_num];
    pthread_t coordinator;

    pthread_create(&coordinator,NULL,coordinator_thread,NULL);


    for( i=0; i<student_num; i++)
    {
        student_id[i]= i+1;
        pthread_create(&students[i],NULL,student_thread,(void*)&student_id[i]);
    }

    for(i=0; i<tutor_num; i++)
    {
        tutor_id[i]= i+student_num+1;
        pthread_create(&tutors[i],NULL,tutor_thread,(void*)&tutor_id[i]);
    }

    pthread_join(coordinator, NULL);

    for(i = 0; i <student_num; i++)
    {
        pthread_join(students[i],NULL);
    }

    for(i = 0; i <tutor_num; i++)
    {
        pthread_join(tutors[i],NULL);
    }
}


void *student_thread(void *student_id)
{
    int id_student=*(int*)student_id;

    while(1)
    {
        if(student_priority[id_student-1]>=help_num)
        {

            pthread_mutex_lock(&seatLock);
            complete++;
            pthread_mutex_unlock(&seatLock);

            //notify coordinate to terminate
            sem_post(&sem_student);

            //student terminate;
            pthread_exit(NULL);
        }

        Sleep(1);

        pthread_mutex_lock(&seatLock);
        if(occupied_chair_num>=chair_num)
        {
            //This Student do not found no empty chair.
            printf("STUDENT:  ");
            printf("Student %d found no empty chair. Will try again later.\n",id_student);
            pthread_mutex_unlock(&seatLock);
            continue;
        }

        occupied_chair_num++;
        totalRequests++;
        newArrivedStudentQueue[id_student-1]=totalRequests;

        //This Student takes a seat.
        printf("STUDENT:  ");
        printf("Student %d takes a seat. Empty chairs = %d\n",id_student,chair_num-occupied_chair_num);
        pthread_mutex_unlock(&seatLock);

        //notify coordinator that student seated.
        sem_post(&sem_student);

        //wait to be tutored.
        while(tutorFinishedQueue[id_student-1]==-1);
        //sem_wait(&sem_tutorToStudent[id_student-1]);

        //St: Student x received help from Tutor y.
        printf("STUDENT:  ");
        printf("Student %d received help from Tutor %d.\n",id_student,tutorFinishedQueue[id_student-1]-student_num);

        //reset the shared data
        pthread_mutex_lock(&tutorFinishedQueueLock);
        tutorFinishedQueue[id_student-1]=-1;
        pthread_mutex_unlock(&tutorFinishedQueueLock);

        //tutored times++ after tutoring.
        pthread_mutex_lock(&seatLock);
        student_priority[id_student-1]++;
        pthread_mutex_unlock(&seatLock);
    }
}


void *coordinator_thread()
{
    while(1)
    {
        //if all students complete, tutors threads and coordinate thread terminate.
        if(complete==student_num)
        {
            int i;
            for(i=0; i<tutor_num; i++)
            {
                //notify tutors to terminate
                sem_post(&sem_coordinator);
            }

            //terminate coordinate thread
            pthread_exit(NULL);
        }

        //wait student's notification
        sem_wait(&sem_student);

        pthread_mutex_lock(&seatLock);
        //find the students who just seated and push them into the priority queue
        int i;
        for(i=0; i<student_num; i++)
        {
            // the student who recently come from student thread
            if(newArrivedStudentQueue[i]>-1)
            {
                priorityQueue[i][0]=student_priority[i];
                priorityQueue[i][1]=newArrivedStudentQueue[i];

                printf("Co-ordinator:  ");
                printf("Student %d, with priority %d in the queue. Waiting students now = %d. Total requests of student = %d\n",student_id[i],student_priority[i],occupied_chair_num,totalRequests);

                //then default this again
                newArrivedStudentQueue[i]=-1;

                //notify tutor that student is in the queue.
                sem_post(&sem_coordinator);
            }
        }
        pthread_mutex_unlock(&seatLock);
    }
}

void *tutor_thread(void *tutor_id)
{
    int id_tutor=*(int*)tutor_id;
    int studentTutoredTimes;
    int studentSequence;
    int id_student;
    int i;

    while(1)
    {
        if(complete==student_num)
        {
            //tutor thread terminate
            pthread_exit(NULL);
        }

       /* for(i=0; i<student_num; i++)
        {
            studentTutoredTimes[i]=help_num[i]-1;
            studentSequence = help_num[i]+studentSequence;
        } */

        studentTutoredTimes=help_num-1;//remain tutor time
        studentSequence=student_num*help_num+1;//total help+1

        id_student=-1;

        //wait coordinator's notification
        sem_wait(&sem_coordinator);

        pthread_mutex_lock(&seatLock);
        //find student with highest priority from priority queue;
        //if students with same tutored times, who come first has higher priority
        //int i;
        for(i=0; i<student_num; i++)
        {
            if(priorityQueue[i][0]>-1 && priorityQueue[i][0]<=studentTutoredTimes  //2nd time higest priority check krtesi
                    && priorityQueue[i][1]<studentSequence)
            {
                studentTutoredTimes=priorityQueue[i][0];//main priority
                studentSequence=priorityQueue[i][1];// total request
               // prio = priorityQueue[i][0];
                id_student=student_id[i];
            }
        }

        //in case no student in the queue.
        if(id_student==-1)
        {
            pthread_mutex_unlock(&seatLock);
            continue;
        }

        priorityQueue[id_student-1][0]=-1;
        priorityQueue[id_student-1][1]=-1;

        occupied_chair_num--;
        //tutoringNow++;
        pthread_mutex_unlock(&seatLock);
        Sleep(1);

        //after tutoring
        pthread_mutex_lock(&seatLock);
        //need to do tutoringNow-- after tutoring.
        //tutoringNow--;
        totalSessions++;
        //Students tutored now = %d.
        printf("\n");

        printf("Tutor:  ");
        printf("Student %d tutored by Tutor %d. Total sessions tutored = %d\n",id_student,id_tutor-student_num,totalSessions);
        tutoringNow++;
        pthread_mutex_unlock(&seatLock);

        //update shared data so student can know who tutored him.
        pthread_mutex_lock(&tutorFinishedQueueLock);
        tutorFinishedQueue[id_student-1]=id_tutor;
        pthread_mutex_unlock(&tutorFinishedQueueLock);
    }
}
