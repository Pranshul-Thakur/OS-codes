#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t teacher_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t student_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t teacher_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t student_cond = PTHREAD_COND_INITIALIZER;

char teacher_queue[100][20];
int teacher_count = 0;
char student_queue[100][20];
int student_count = 0;

void *student(void *name) {
    char *student_name = (char *)name;

    pthread_mutex_lock(&student_mutex);
    strcpy(student_queue[student_count], student_name);
    student_count++;
    pthread_mutex_unlock(&student_mutex);

    while (1) {
        pthread_mutex_lock(&teacher_mutex);
        if (teacher_count > 0) {
            char teacher_name[20];
            strcpy(teacher_name, teacher_queue[0]);
            teacher_count--;
            printf("%s is being served.\n", teacher_name);
            pthread_cond_signal(&teacher_cond);
            pthread_mutex_unlock(&teacher_mutex);
            printf("%s is issued a book.\n", student_name);
            break;
        }
        pthread_mutex_unlock(&teacher_mutex);
    }

    pthread_exit(NULL);
}

void *teacher(void *name) {
    char *teacher_name = (char *)name;

    pthread_mutex_lock(&teacher_mutex);
    strcpy(teacher_queue[teacher_count], teacher_name);
    teacher_count++;
    printf("%s has arrived at the counter.\n", teacher_name);
    pthread_mutex_unlock(&teacher_mutex);

    pthread_mutex_lock(&student_mutex);
    if (student_count > 0) {
        char student_name[20];
        strcpy(student_name, student_queue[0]);
        student_count--;
        printf("%s is being served.\n", student_name);
        pthread_cond_signal(&student_cond);
        pthread_mutex_unlock(&student_mutex);
        printf("%s issued a book.\n", teacher_name);
    } else {
        pthread_mutex_unlock(&student_mutex);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t student1, student2, teacher1, teacher2;
    char student_name1[] = "Student 1";
    char student_name2[] = "Student 2";
    char teacher_name1[] = "Teacher 1";
    char teacher_name2[] = "Teacher 2";

    pthread_create(&student1, NULL, student, (void *)student_name1);
    pthread_create(&student2, NULL, student, (void *)student_name2);
    pthread_create(&teacher1, NULL, teacher, (void *)teacher_name1);
    pthread_create(&teacher2, NULL, teacher, (void *)teacher_name2);

    pthread_join(student1, NULL);
    pthread_join(student2, NULL);
    pthread_join(teacher1, NULL);
    pthread_join(teacher2, NULL);

    return 0;
}

