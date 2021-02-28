#include <iostream>
#include <fstream>
#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_spmatrix.h>
#include <gsl/gsl_splinalg.h>
#include <vector>

// y: 0-N; x: 0-M
const int M = 300, N = 100;
bool breakdown[M][N];
bool ground[M][N];

int cur_breakdown = 0;
std::vector<int> breakdown_x;
std::vector<int> breakdown_y;

gsl_vector * u;

void pin(int n) {
    int i = M/2;
    int j = N-1;
    for (int a = 0; a < n; ++a) {
        breakdown[i][j] = 1;
        breakdown_x.push_back(i);
        breakdown_y.push_back(j);
        cur_breakdown += 1;
        j--;
    }
}

void global_init() {
    // init breakdown point
    memset(breakdown, 0, sizeof(breakdown));
    pin(10);
    
    // init ground
    memset(ground, 0, sizeof(ground));
    for (int i = 170; i < 180; ++i) {
        for (int j = 0; j < 40; ++j) {
            ground[i][j] = 1;
        }
    }
    
    // init solution vector
    u = gsl_vector_calloc(M*N);
}

int max_step = 200;

void step() {
    int eq_total = M*N;
    
    gsl_vector * b = gsl_vector_calloc(eq_total);
    gsl_spmatrix * A = gsl_spmatrix_alloc_nzmax(eq_total, eq_total, eq_total * 5, GSL_SPMATRIX_TRIPLET);
    
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            if (breakdown[i][j]) {
                gsl_spmatrix_set(A, i * N + j, i * N + j, 1);
                gsl_vector_set(b, i * N + j, 1);
            } else if (ground[i][j]) {
                gsl_spmatrix_set(A, i * N + j, i * N + j, 1);
                gsl_vector_set(b, i * N + j, 0);
            } else {
                double vb = 0;
                // mid
                gsl_spmatrix_set(A, i * N + j, i * N + j, -4);
                // left
                if (i == 0) {
                    
                } else if (breakdown[i-1][j]) {
                    vb += -1;
                } else if (ground[i-1][j]) {
                    
                } else {
                    gsl_spmatrix_set(A, i * N + j, (i-1) * N + j, 1);
                }
                // right
                if (i == M-1) {
                    
                } else if (breakdown[i+1][j]) {
                    vb += -1;
                } else if (ground[i+1][j]) {
                    
                } else {
                    gsl_spmatrix_set(A, i * N + j, (i+1) * N + j, 1);
                }
                // down
                if (j == 0) {
                    
                } else if (breakdown[i][j-1]) {
                    vb += -1;
                } else if (ground[i][j-1]) {
                    
                } else {
                    gsl_spmatrix_set(A, i * N + j, i * N + j-1, 1);
                }
                // up
                if (j == N-1) {
                    vb += -1;
                } else if (breakdown[i][j+1]) {
                    vb += -1;
                } else if (ground[i][j+1]) {
                    
                } else {
                    gsl_spmatrix_set(A, i * N + j, i * N + j+1, 1);
                }
                gsl_vector_set(b, i * N + j, vb);
            }
        }
    }
    
    double tol = 1e-3;
    size_t max_step = 20;
    gsl_splinalg_itersolve * work = gsl_splinalg_itersolve_alloc(gsl_splinalg_itersolve_gmres, eq_total, 0);
    int status;
    do {
        status = gsl_splinalg_itersolve_iterate(A, b, tol, u, work);
    } while (status == GSL_CONTINUE && max_step--);
    gsl_splinalg_itersolve_free(work);
    
    struct Point_Prob {
        int i, j;
        double p;
    };
    std::vector<Point_Prob> p;
    
    for (int a = 0; a < cur_breakdown; ++a) {
        int i = breakdown_x[a];
        int j = breakdown_y[a];
        if (i == 0 || breakdown[i-1][j] || ground[i-1][j]) {
        } else {
            p.push_back({
                i-1,
                j,
                pow(1 - gsl_vector_get(u, (i-1) * N + j), 3)
            });
        }
        if (i == M-1 || breakdown[i+1][j] || ground[i+1][j]) {
        } else {
            p.push_back({
                i+1,
                j,
                pow(1 - gsl_vector_get(u, (i+1) * N + j), 3)
            });
        }
        if (j == 0 || breakdown[i][j-1] || ground[i][j-1]) {
        } else {
            p.push_back({
                i,
                j-1,
                pow(1 - gsl_vector_get(u, i * N + j-1), 3)
            });
        }
        if (j == N-1 || breakdown[i][j+1] || ground[i][j+1]) {
        } else {
            p.push_back({
                i,
                j+1,
                pow(1 - gsl_vector_get(u, i * N + j+1), 3)
            });
        }
    }
    
    double sum = 0;
    for (auto & e : p) {
        sum += e.p;
    }
    for (auto & e : p) {
        e.p /= sum;
    }
    
    double r = (double)rand() / (double)RAND_MAX;
    auto rb = p.begin();
    int x = rb->i;
    int y = rb->j;
    for (auto & e : p) {
        if (r < e.p) {
            x = e.i;
            y = e.j;
            break;
        } else {
            r -= e.p;
        }
    }
    
    
    breakdown[x][y] = 1;
    breakdown_x.push_back(x);
    breakdown_y.push_back(y);
    cur_breakdown++;
    
    gsl_spmatrix_free(A);
    gsl_vector_free(b);
}

int main() {
    srand((unsigned)time(0));
    global_init();
    for (int a = 0; a < max_step; ++a) {
        printf("step: %d\n", a);
        step();
    }
    FILE * outx = fopen("./outx.txt", "w");
    FILE * outy = fopen("./outy.txt", "w");
    for (int i : breakdown_x) {
        fprintf(outx, "%d\n", i);
    }
    for (int i : breakdown_y) {
        fprintf(outy, "%d\n", i);
    }
    fclose(outx);
    fclose(outy);
    return 0;
}
