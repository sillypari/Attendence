#include <stdio.h>
#include <string.h>
#include <time.h>

#define TOTAL_STUDENTS 60

// Function to export attendance to a text file
void exportAttendanceToFile(char names[][50], int present[], int totalPresent, int totalAbsent, const char *subject) {
    // Get the current date and time
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char dateStr[100];
    strftime(dateStr, sizeof(dateStr), "%Y%m%d", t);  // Date format: YYYYMMDD
    
    // Generate the file name based on the format {ATDNC}{Date}{Subject}.txt
    char filename[200];
    snprintf(filename, sizeof(filename), "ATDNC%s%s.txt", dateStr, subject);

    // Open the file for writing
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    // Write the title and attendance details
    fprintf(file, "Attendance for %s\n", subject);
    
    // Write the date and time
    char timeStr[100];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", t);
    fprintf(file, "Date and Time: %s\n", timeStr);

    // Write attendance statistics
    fprintf(file, "Total Students Present: %d\n", totalPresent);
    fprintf(file, "Total Students Absent: %d\n", totalAbsent);

    // List of absent students
    fprintf(file, "\nList of Absent Students:\n");
    for (int i = 0; i < TOTAL_STUDENTS; i++) {
        if (present[i] == 0) {  // If student is absent
            fprintf(file, "%s\n", names[i]);
        }
    }

    fclose(file);
    printf("Attendance exported to '%s'.\n", filename);
}

// Function to read names from the text file
int readStudentNames(char names[][50], const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening student names file! Ensure the file exists and the path is correct.\n");
        return 0;
    }

    char buffer[100];
    int i = 0;
    while (fgets(buffer, sizeof(buffer), file) && i < TOTAL_STUDENTS) {
        char title[10];  // To store Mr or Ms
        // Extract the full name after "Mr" or "Ms"
        int result = sscanf(buffer, "%s %[^\n]", title, names[i]);
        
        // Debugging output
        printf("Debug: Read line: %s", buffer);
        
        if (result == 2) {
            printf("Debug: Parsed student name: %s\n", names[i]);
            i++;
        } else {
            printf("Warning: Unable to parse line: %s", buffer);
        }
    }

    fclose(file);
    return i;  // Return the number of students read
}

int main() {
    char names[TOTAL_STUDENTS][50];
    int present[TOTAL_STUDENTS];
    int totalPresent = 0;
    int totalAbsent = 0;

    // Ask for the subject name
    char subject[50];
    printf("Enter the subject name: ");
    fgets(subject, sizeof(subject), stdin);
    subject[strcspn(subject, "\n")] = 0;  // Remove the newline character

    // Read student names from the file
    int totalStudents = readStudentNames(names, "students.txt");
    if (totalStudents != TOTAL_STUDENTS) {
        printf("Error: The number of students read is not %d. Ensure the file has the correct format.\n", TOTAL_STUDENTS);
        return 1;
    }

    // Take attendance input
    for (int i = 0; i < TOTAL_STUDENTS; i++) {
        char attendance[3];  // Array to capture the input line
        
        int validInput = 0;
        
        while (!validInput) {
            printf("Enter attendance for %s (Y for Present, N for Absent, or press Enter for Present): ", names[i]);
            fgets(attendance, sizeof(attendance), stdin);  // Get the full line of input
            
            // Check if the input is empty (meaning just Enter was pressed)
            if (attendance[0] == '\n' || attendance[0] == 'Y' || attendance[0] == 'y') {
                present[i] = 1;  // Mark as present
                totalPresent++;
                validInput = 1;
            } 
            // Check for Absent input
            else if (attendance[0] == 'N' || attendance[0] == 'n') {
                present[i] = 0;  // Mark as absent
                totalAbsent++;
                validInput = 1;
            } 
            else {
                printf("Invalid input. Please enter 'Y' for present, 'N' for absent, or press Enter for present.\n");
            }
        }
    }

    // Export attendance to a text file
    exportAttendanceToFile(names, present, totalPresent, totalAbsent, subject);

    return 0;
}
