#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #define CLEAR_CMD "cls"
#else
    #define CLEAR_CMD "clear"
#endif

#define FILE_NAME "todo_list.txt"

void clearScreen(void) {
    system(CLEAR_CMD);
}

void stripNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void initializeFile(void) {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        file = fopen(FILE_NAME, "w");
        if (file) {
            fprintf(file, "Sample Task 1\n");
            fprintf(file, "Sample Task 2\n");
            fclose(file);
        }
    } else {
        fclose(file);
    }
}

void printMenu(void) {
    printf("\033[1;31m===== To-Do Application =====\033[0m\n");
    printf("1. Create New To-Do List\n");
    printf("2. View All To-Do Lists\n");
    printf("3. Mark Item as Completed\n");
    printf("4. Delete a To-Do Item\n");
    printf("5. Exit\n");
    printf("Choose an option: ");
}

int getMenuChoice(void) {
    char buffer[256];
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return 0;
    }
    stripNewline(buffer);

    int choice;
    char leftover;
    int parsed = sscanf(buffer, "%d%c", &choice, &leftover);
    if (parsed != 1 || choice < 1 || choice > 5) {
        return 0;
    }

    return choice;
}

void viewToDoList(void) {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        printf("Error opening to-do list file.\n");
        return;
    }

    printf("\033[1;31m=== To-Do List ===\033[0m\n");
    char line[256];
    int count = 1;
    while (fgets(line, sizeof(line), file)) {
        stripNewline(line);
        printf("%d. %s\n", count++, line);
    }
    fclose(file);
}

void addToDoItem(void) {
    char newTask[256];
    printf("Enter new to-do item: ");
    fgets(newTask, sizeof(newTask), stdin);
    stripNewline(newTask);

    FILE *file = fopen(FILE_NAME, "a");
    if (!file) {
        printf("Error opening to-do list file.\n");
        return;
    }
    fprintf(file, "%s\n", newTask);
    fclose(file);
    printf("Task added successfully!\n");
}

void markItemAsCompleted(void) {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        printf("Error opening to-do list file.\n");
        return;
    }

    char tasks[100][256];
    int count = 0;

    while (fgets(tasks[count], sizeof(tasks[count]), file) && count < 100) {
        stripNewline(tasks[count]);
        count++;
    }
    fclose(file);

    if (count == 0) {
        printf("No tasks to complete.\n");
        return;
    }

    printf("Select the task number to mark as completed:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, tasks[i]);
    }

    printf("Enter number: ");
    int index;
    scanf("%d", &index);
    getchar(); // Consume leftover newline

    if (index < 1 || index > count) {
        printf("Invalid selection.\n");
        return;
    }

    if (strncmp(tasks[index - 1], "[X] ", 4) == 0) {
        printf("Task already marked as completed.\n");
        return;
    }

    char completedTask[256];
    snprintf(completedTask, sizeof(completedTask), "[X] %s", tasks[index - 1]);
    strncpy(tasks[index - 1], completedTask, sizeof(tasks[index - 1]));

    file = fopen(FILE_NAME, "w");
    if (!file) {
        printf("Error writing to file.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s\n", tasks[i]);
    }
    fclose(file);

    printf("Task marked as completed!\n");
}

void deleteToDoItem(void) {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        printf("Error opening to-do list file.\n");
        return;
    }

    char tasks[100][256];
    int count = 0;

    while (fgets(tasks[count], sizeof(tasks[count]), file) && count < 100) {
        stripNewline(tasks[count]);
        count++;
    }
    fclose(file);

    if (count == 0) {
        printf("No tasks to delete.\n");
        return;
    }

    printf("Select the task number to delete:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, tasks[i]);
    }

    printf("Enter number: ");
    int index;
    scanf("%d", &index);
    getchar(); // Consume newline

    if (index < 1 || index > count) {
        printf("Invalid selection.\n");
        return;
    }

    file = fopen(FILE_NAME, "w");
    if (!file) {
        printf("Error writing to file.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        if (i != index - 1) {
            fprintf(file, "%s\n", tasks[i]);
        }
    }
    fclose(file);
    printf("Task deleted successfully!\n");
}

void handleChoice(int choice) {
    clearScreen();
    printf("\033[1;31m===== To-Do Application =====\033[0m\n");

    switch (choice) {
        case 1:
            addToDoItem();
            break;
        case 2:
            viewToDoList();
            break;
        case 3:
            markItemAsCompleted();
            break;
        case 4:
            deleteToDoItem();
            break;
        case 5:
            printf("Exiting...\n");
            exit(0);
    }
}

int main(void) {
    initializeFile();
    while (1) {
        clearScreen();
        printMenu();

        int choice = getMenuChoice();
        if (choice == 0) {
            printf("\nInvalid input (not an option). Press Enter to continue...\n");
            char dummy[256];
            fgets(dummy, sizeof(dummy), stdin);
            continue;
        }

        handleChoice(choice);

        printf("\nPress Enter to return to the main menu...");
        char pauseBuffer[256];
        fgets(pauseBuffer, sizeof(pauseBuffer), stdin);
    }
    return 0;
}
