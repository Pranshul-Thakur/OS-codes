#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

// Semaphores for pen, paper, and question paper
sem_t pen_semaphore;
sem_t paper_semaphore;
sem_t question_paper_semaphore;

// Semaphore for assignment synchronization
sem_t assignment_semaphore;

void *student(void *name);
void teacher(void);

int main() {
    pthread_t student1_thread, student2_thread, student3_thread, teacher_thread;

    // Initialize semaphores
    sem_init(&pen_semaphore, 0, 1);
    sem_init(&paper_semaphore, 0, 1);
    sem_init(&question_paper_semaphore, 0, 1);
    sem_init(&assignment_semaphore, 0, 1);

    // Start threads
    pthread_create(&student1_thread, NULL, student, (void *)"Student 1");
    pthread_create(&student2_thread, NULL, student, (void *)"Student 2");
    pthread_create(&student3_thread, NULL, student, (void *)"Student 3");
    pthread_create(&teacher_thread, NULL, (void *)teacher, NULL);


    // Wait for all threads to finish
    pthread_join(student1_thread, NULL);
    pthread_join(student2_thread, NULL);
    pthread_join(student3_thread, NULL);
    pthread_join(teacher_thread, NULL);

    // Destroy semaphores
    sem_destroy(&pen_semaphore);
    sem_destroy(&paper_semaphore);
    sem_destroy(&question_paper_semaphore);
    sem_destroy(&assignment_semaphore);

    return 0;
}

void *student(void *name) {
    char *student_name = (char *)name;

    while (1) {
        sem_wait(&pen_semaphore);
        if (sem_trywait(&paper_semaphore) != 0) {
            sem_post(&pen_semaphore);
            continue;
        }

        char item3[] = "question paper";
        sem_wait(&question_paper_semaphore);

        printf("%s has pen, paper, and %s. Making the assignment.\n", student_name, item3);

        sem_wait(&assignment_semaphore);
        printf("%s has completed the assignment.\n", student_name);
        sem_post(&assignment_semaphore);

        sem_post(&question_paper_semaphore);
        sem_post(&pen_semaphore);
        sem_post(&paper_semaphore);
        break;
    }

    pthread_exit(NULL);
}

void teacher() {
    while (1) {
        char item1[] = "pen";
        char item2[] = "paper";
        char item3[] = "question paper";

        sem_post(&pen_semaphore);
        sem_post(&paper_semaphore);
        sem_post(&question_paper_semaphore);

        printf("Teacher has placed pen and paper on the table.\n");
        student("Student 1");

        sem_post(&pen_semaphore);
        sem_post(&paper_semaphore);
        sem_post(&question_paper_semaphore);

        printf("Teacher has placed pen and question paper on the table.\n");
        student("Student 2");

        sem_post(&pen_semaphore);
        sem_post(&paper_semaphore);
        sem_post(&question_paper_semaphore);

        printf("Teacher has placed paper and question paper on the table.\n");
        student("Student 3");
    }
}
