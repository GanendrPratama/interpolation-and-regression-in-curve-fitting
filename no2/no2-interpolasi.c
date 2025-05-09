#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Constants for array sizes
#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES     60   // Maximum number of data points
#define MAX_DEGREE      60   // Max degree of interpolated polynomial
#define skip_lines     34   // Number of lines to skip in the CSV file

// Function Prototypes
void multiplyPoly(double a[], int degA, double b[], int degB, double result[]);
void addPoly(double a[], int degA, double b[], int degB, double result[]);
void buildLagrangeTerm(int i, double x[], int n, double L[]);
void lagrangeInterpolation(double x[], double y[], int n, double result[]);
int getPolyDegree(double poly[], int maxDegree);
void printPolynomial(double poly[], int degree);
int read_csv_data(const char *filename, double year[], double population[], double percentage[]);

/**
 * Multiplies two polynomials represented as arrays.
 * Example: [1, 2] * [3, 4] = [3, 10, 8]
 */
void multiplyPoly(double a[], int degA, double b[], int degB, double result[]) {
    memset(result, 0, (MAX_DEGREE) * sizeof(double));
    for (int i = 0; i <= degA; i++) {
        for (int j = 0; j <= degB; j++) {
            result[i + j] += a[i] * b[j];
        }
    }
}

/**
 * Adds two polynomials together.
 * Example: [1, 2] + [3, 4] = [4, 6]
 */
void addPoly(double a[], int degA, double b[], int degB, double result[]) {
    int maxDeg = (degA > degB) ? degA : degB;
    for (int i = 0; i <= maxDeg && i < MAX_DEGREE; i++) {
        result[i] = a[i] + b[i];
    }
}

/**
 * Builds the Lagrange basis polynomial L_i(x)
 * This is one of the key steps in Lagrange Interpolation.
 *
 * For each point i, we compute:
 * L_i(x) = product over j≠i of (x - x_j)/(x_i - x_j)
 */
void buildLagrangeTerm(int i, double x[], int n, double L[]) {
    double temp[MAX_DEGREE] = {0}, term[MAX_DEGREE] = {0};

    // Start with the constant polynomial "1"
    L[0] = 1.0;
    for (int d = 1; d < MAX_DEGREE; d++) L[d] = 0.0;

    for (int j = 0; j < n; j++) {
        if (i == j) continue;

        // Compute denominator: x_i - x_j
        double denom = x[i] - x[j];

        // Build linear factor: (x - x_j)
        term[0] = -x[j]; 
        term[1] = 1.0;

        // Normalize by dividing by denominator
        for (int k = 0; k <= 1; k++) term[k] /= denom;

        // Multiply current L with this term
        multiplyPoly(L, getPolyDegree(L, MAX_DEGREE), term, 1, temp);
        memcpy(L, temp, MAX_DEGREE * sizeof(double));
    }
}

/**
 * Performs full Lagrange Interpolation on given data points.
 * Constructs the interpolating polynomial P(x) = sum(y_i * L_i(x))
 */
void lagrangeInterpolation(double x[], double y[], int n, double result[]) {
    double Li[MAX_DEGREE] = {0}, scaledLi[MAX_DEGREE] = {0}, temp[MAX_DEGREE] = {0};

    // Initialize result to zero
    memset(result, 0, MAX_DEGREE * sizeof(double));

    for (int i = 0; i < n; i++) {
        // Build Lagrange basis polynomial L_i(x)
        memset(Li, 0, MAX_DEGREE * sizeof(double));
        buildLagrangeTerm(i, x, n, Li);

        // Scale L_i(x) by y_i
        for (int j = 0; j < MAX_DEGREE; j++) {
            scaledLi[j] = Li[j] * y[i];
        }

        // Add scaledLi to the final result
        memset(temp, 0, MAX_DEGREE * sizeof(double));
        addPoly(result, n - 1, scaledLi, n - 1, temp);
        memcpy(result, temp, MAX_DEGREE * sizeof(double));
    }
}

/**
 * Returns the actual degree of the polynomial (ignores trailing zeros)
 */
int getPolyDegree(double poly[], int maxDegree) {
    int degree = 0;
    for (int i = maxDegree; i >= 0; i--) {
        if (fabs(poly[i]) > 1e-10) {
            degree = i;
            break;
        }
    }
    return degree;
}

/**
 * Prints the polynomial in a readable form
 */
void printPolynomial(double poly[], int degree) {
    printf("P(x) = ");
    int first = 1;

    for (int i = degree; i >= 0; i--) {
        double coeff = poly[i];
        if (fabs(coeff) < 1e-10) continue;

        if (!first) {
            if (coeff >= 0) printf(" + ");
            else printf(" - ");
        } else {
            first = 0;
            if (coeff < 0) printf("-");
        }

        double absCoeff = fabs(coeff);

        if (i == 0) {
            printf("%.6f", absCoeff);
        } else if (i == 1) {
            if (absCoeff == 1.0)
                printf("x");
            else
                printf("%.6fx", absCoeff);
        } else {
            if (absCoeff == 1.0)
                printf("x^%d", i);
            else
                printf("%.6fx^%d", absCoeff, i);
        }
    }

    if (first) {
        printf("0");
    }

    printf("\n");
}

/**
 * Reads data from CSV file.
 * Skips header and first 34 rows, then reads up to 25 entries.
 *
 * Columns used:
 * - Year (independent variable)
 * - Percentage_Internet_User (dependent variable for one interpolation)
 * - Population (dependent variable for another interpolation)
 * 
 * Using too many entries will make the output osciliate.
 * This is a known issue with Lagrange interpolation or any polynomial interpolation.
 * The more points you use, the more oscillation you get.
 * This is called Runge's phenomenon
 */
int read_csv_data(const char *filename, double year[], double population[], double percentage[]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening file");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    int count = 0;

    // Skip header
    if (fgets(line, sizeof(line), fp)) { /* skip header */ }

    // Skipping the "skip_lines" lines
    for (int i = 0; i < skip_lines && fgets(line, sizeof(line), fp); i++) {
        // Just skip
    }

    // Read remaining lines into arrays
    while (fgets(line, sizeof(line), fp) && count < 25) {
        char *token = strtok(line, ",");
        if (token != NULL) {
            year[count] = atof(token);  // First column: Year

            token = strtok(NULL, ",");  // Second column: Percentage_Internet_User
            if (token != NULL) {
                percentage[count] = atof(token);
            }

            token = strtok(NULL, ",");  // Third column: Population
            if (token != NULL) {
                population[count] = atof(token);
                count++;
            }
        }
    }

    fclose(fp);

    if (count == 0) {
        printf("No data was read.\n");
        return -1;
    }

    return count;
}

/**
 * Main function: demonstrates Lagrange interpolation
 * - First for Percentage_Internet_User vs Year
 * - Then for Population vs Year
 */
int main() {
    double year[MAX_ENTRIES];
    double population[MAX_ENTRIES];
    double percentage[MAX_ENTRIES];

    int entries = read_csv_data("../Data Tugas Pemrograman A.csv", year, population, percentage);

    if (entries <= 0) {
        printf("Failed to load data.\n");
        return 1;
    }

    printf("Using %d data points:\n", entries);
    for (int i = 0; i < entries; i++) {
        printf("Year: %.0f | Internet Users: %.6f%% | Population: %.0f\n",
               year[i], percentage[i], population[i]);
    }

    printf("\n--- Interpolating: Percentage of Internet Users vs Year ---\n\n");

    double poly_percent[MAX_DEGREE] = {0};
    lagrangeInterpolation(year, percentage, entries, poly_percent);

    int degree_percent = getPolyDegree(poly_percent, entries - 1);
    printf("Interpolated Polynomial for Internet Usage (%):\n");
    printPolynomial(poly_percent, degree_percent);

    printf("\n--- Interpolating: Population vs Year ---\n\n");

    double poly_population[MAX_DEGREE] = {0};
    lagrangeInterpolation(year, population, entries, poly_population);

    int degree_pop = getPolyDegree(poly_population, entries - 1);
    printf("Interpolated Polynomial for Population:\n");
    printPolynomial(poly_population, degree_pop);

    return 0;
}