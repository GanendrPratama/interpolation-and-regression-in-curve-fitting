#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRIES 60

// Function to perform linear regression
void linear_regression(double x[], double y[], int n, double *slope, double *intercept) {
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_xx = 0;

    for (int i = 0; i < n; i++) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_xx += x[i] * x[i];
    }

    double denominator = n * sum_xx - sum_x * sum_x;
    if (denominator == 0) {
        *slope = 0;
        *intercept = 0;
        return;
    }

    *slope = (n * sum_xy - sum_x * sum_y) / denominator;
    *intercept = (sum_y - (*slope) * sum_x) / n;
}

// Function to read data from CSV
int read_csv_data(const char *filename, double year[], double population[], double percentage[]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening file");
        return -1;
    }

    char line[1024];
    int count = 0;

    // Skip header
    if (fgets(line, sizeof(line), fp)) { /* skip header */ }

    while (fgets(line, sizeof(line), fp) && count < MAX_ENTRIES) {
        char *token = strtok(line, ",");
        if (token != NULL) {
            year[count] = atof(token);

            token = strtok(NULL, ",");
            if (token != NULL) {
                percentage[count] = atof(token);
            }

            token = strtok(NULL, ",");
            if (token != NULL) {
                population[count] = atof(token);
            }

            count++;
        }
    }

    fclose(fp);

    return count;
}

int main() {
    double year[MAX_ENTRIES];
    double population[MAX_ENTRIES];
    double percentage[MAX_ENTRIES];

    int entries = read_csv_data("../Data Tugas Pemrograman A.csv", year, population, percentage);

    if (entries <= 0) {
        printf("Failed to load data.\n");
        return 1;
    }

    printf("Loaded %d data points.\n\n", entries);

    // --- Regression 1: Internet Usage (%) vs Year ---
    double slope_p, intercept_p;
    linear_regression(year, percentage, entries, &slope_p, &intercept_p);

    printf("Regression Model: Internet Usage (%%) vs Year\n");
    printf("y = %.6f * x + %.6f\n\n", slope_p, intercept_p);

    // --- Regression 2: Population vs Year ---
    double slope_pop, intercept_pop;
    linear_regression(year, population, entries, &slope_pop, &intercept_pop);

    printf("Regression Model: Population vs Year\n");
    printf("y = %.2f * x + %.2f\n\n", slope_pop, intercept_pop);

    // Optional: Predict values
    double predict_year = 2025;
    double predicted_percent = slope_p * predict_year + intercept_p;
    double predicted_population = slope_pop * predict_year + intercept_pop;

    printf("Predicted Internet Usage in %.0f: %.2f%%\n", predict_year, predicted_percent);
    printf("Predicted Population in %.0f: %.0f\n", predict_year, predicted_population);

    return 0;
}