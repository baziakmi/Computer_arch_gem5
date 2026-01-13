#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    char name[100];
    float CPI;
    float L1d_miss_rate;
    float L1i_miss_rate;
    float L2_miss_rate;
    float L1d_accesses;
    float L1i_accesses;
    float L2_accesses;
    float sim_seconds;
} BenchmarkData;

float dynamic_energy(float mem_size, float mem_assoc, float mem_accesses);
float static_energy(float mem_size, float sim_time, bool L1, bool L1d);
float total_energy(float CPI, float L1d_access, float L1d_size, float L1d_assoc, float L1i_access, float L1i_size, float L1i_assoc, float L2_access, float L2_size, float L2_assoc, float tsim);

int main() {
    // Data from CPI_Final_Results.txt
    BenchmarkData results[] = {
        {"bzip2_L1s64kB_L1a4_L2s2MB", 1.660740, 0.013346, 0.000067, 0.316569, 52159168, 10236307, 638091, 0.083037},
        {"bzip2_L1s64kB_L1a4_L2s4MB", 1.635076, 0.013344, 0.000067, 0.283052, 52165952, 10236301, 638091, 0.081754},
        {"bzip2_L1s64kB_L1a8_L2s2MB", 1.654375, 0.012738, 0.000066, 0.333120, 52158838, 10236300, 606382, 0.082719},
        {"bzip2_L1s64kB_L1a8_L2s4MB", 1.628651, 0.012736, 0.000066, 0.297854, 52165391, 10236292, 606381, 0.081433},
        {"bzip2_L1s128kB_L1a4_L2s2MB", 1.637125, 0.010759, 0.000066, 0.401497, 52156802, 10236082, 503114, 0.081856},
        {"bzip2_L1s128kB_L1a4_L2s4MB", 1.611404, 0.010757, 0.000066, 0.358990, 52163487, 10236072, 503114, 0.080570},
        {"bzip2_L1s128kB_L1a8_L2s2MB", 1.632979, 0.010290, 0.000066, 0.422018, 52156459, 10236065, 478645, 0.081649},
        {"bzip2_L1s128kB_L1a8_L2s4MB", 1.607446, 0.010288, 0.000066, 0.377340, 52163100, 10236057, 478648, 0.080372},
        {"mcf_L1s64kB_L1a4_L2s2MB", 1.107753, 0.001930, 0.000019, 0.765596, 35526700, 26974215, 50686, 0.055388},
        {"mcf_L1s64kB_L1a4_L2s4MB", 1.107436, 0.001930, 0.000019, 0.762755, 35526700, 26974284, 50686, 0.055372},
        {"mcf_L1s64kB_L1a8_L2s2MB", 1.107701, 0.001914, 0.000019, 0.771609, 35526700, 26974165, 50291, 0.055385},
        {"mcf_L1s64kB_L1a8_L2s4MB", 1.107384, 0.001914, 0.000019, 0.768746, 35526700, 26974231, 50291, 0.055369},
        {"mcf_L1s128kB_L1a4_L2s2MB", 1.107390, 0.001846, 0.000019, 0.798079, 35526700, 26974149, 48623, 0.055369},
        {"mcf_L1s128kB_L1a4_L2s4MB", 1.107072, 0.001846, 0.000019, 0.795118, 35526700, 26974212, 48623, 0.055354},
        {"mcf_L1s128kB_L1a8_L2s2MB", 1.107389, 0.001839, 0.000019, 0.801061, 35526700, 26974128, 48442, 0.055369},
        {"mcf_L1s128kB_L1a8_L2s4MB", 1.107072, 0.001839, 0.000019, 0.798088, 35526700, 26974191, 48442, 0.055354},
        {"sjeng_L1s64kB_L1a4_L2s2MB", 9.875443, 0.121829, 0.000018, 0.999985, 86382111, 32134629, 5263906, 0.493772},
        {"sjeng_L1s64kB_L1a4_L2s4MB", 9.871248, 0.121829, 0.000018, 0.999985, 86382111, 32134633, 5263906, 0.493562},
        {"sjeng_L1s64kB_L1a8_L2s2MB", 9.875515, 0.121829, 0.000018, 0.999986, 86382111, 32134636, 5263903, 0.493776},
        {"sjeng_L1s64kB_L1a8_L2s4MB", 9.871093, 0.121829, 0.000018, 0.999986, 86382111, 32134636, 5263903, 0.493555},
        {"sjeng_L1s128kB_L1a4_L2s2MB", 9.875216, 0.121829, 0.000018, 0.999987, 86382111, 32134634, 5263895, 0.493761},
        {"sjeng_L1s128kB_L1a4_L2s4MB", 9.871404, 0.121829, 0.000018, 0.999987, 86382111, 32134634, 5263895, 0.493570},
        {"sjeng_L1s128kB_L1a8_L2s2MB", 9.875216, 0.121829, 0.000018, 0.999987, 86382111, 32134637, 5263898, 0.493761},
        {"sjeng_L1s128kB_L1a8_L2s4MB", 9.871404, 0.121829, 0.000018, 0.999987, 86382111, 32134637, 5263898, 0.493570},
        {"lbm_L1s64kB_L1a4_L2s2MB", 3.435670, 0.060971, 0.000086, 0.999980, 48795047, 6046201, 1488129, 0.171784},
        {"lbm_L1s64kB_L1a4_L2s4MB", 3.431821, 0.060971, 0.000086, 0.999980, 48795047, 6046201, 1488129, 0.171591},
        {"lbm_L1s64kB_L1a8_L2s2MB", 3.435670, 0.060971, 0.000085, 0.999983, 48795048, 6046205, 1488125, 0.171784},
        {"lbm_L1s64kB_L1a8_L2s4MB", 3.431933, 0.060971, 0.000085, 0.999983, 48795048, 6046205, 1488125, 0.171597},
        {"lbm_L1s128kB_L1a4_L2s2MB", 3.435670, 0.060971, 0.000085, 0.999983, 48795048, 6046205, 1488125, 0.171784},
        {"lbm_L1s128kB_L1a4_L2s4MB", 3.431933, 0.060971, 0.000085, 0.999983, 48795048, 6046205, 1488125, 0.171597},
        {"lbm_L1s128kB_L1a8_L2s2MB", 3.435670, 0.060971, 0.000085, 0.999983, 48795048, 6046205, 1488125, 0.171784},
        {"lbm_L1s128kB_L1a8_L2s4MB", 3.431933, 0.060971, 0.000085, 0.999983, 48795048, 6046205, 1488125, 0.171597}
    };

    int num_rows = sizeof(results) / sizeof(results[0]);

    // Print Header
    printf("%-30s | %-8s | %-10s | %-10s | %-10s | %-10s | %-12s | %-12s\n", 
           "Benchmark", "CPI", "L1d Miss", "L1i Miss", "L2 Miss", "t_sim (s)", "E_total (J)", "CPI * E_total");
    printf("--------------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < num_rows; i++) {
        // Variables to extract from name
        float l1_size_kb = 0;
        float l1_assoc = 0;
        float l2_size_mb = 0;
        float l2_assoc = 8; // Defaulting L2 associativity to 8 as it's not in the name

        // Parsing the string "benchmark_L1sXXkB_L1aX_L2sXMB"
        char *name = results[i].name;
        if (strstr(name, "L1s64kB")) l1_size_kb = 64;
        else if (strstr(name, "L1s128kB")) l1_size_kb = 128;

        if (strstr(name, "L1a4")) l1_assoc = 4;
        else if (strstr(name, "L1a8")) l1_assoc = 8;

        if (strstr(name, "L2s2MB")) l2_size_mb = 2;
        else if (strstr(name, "L2s4MB")) l2_size_mb = 4;

        // convert to bytes
        float l1_size_bytes = l1_size_kb * 1024.0;
        float l2_size_bytes = l2_size_mb * 1024.0 * 1024.0;

        // Calculate Energy
        float E_total = total_energy(results[i].CPI, 
                                     results[i].L1d_accesses, l1_size_bytes, l1_assoc,
                                     results[i].L1i_accesses, l1_size_bytes, l1_assoc,
                                     results[i].L2_accesses, l2_size_bytes, l2_assoc,
                                     results[i].sim_seconds);
        
        float EDP = results[i].CPI * E_total; 

        // Print Row with all requested columns
        printf("%-30s | %8.4f | %10.6f | %10.6f | %10.6f | %10.4f | %12.6f | %12.6f|\n", 
               name, 
               results[i].CPI, 
               results[i].L1d_miss_rate, 
               results[i].L1i_miss_rate, 
               results[i].L2_miss_rate, 
               results[i].sim_seconds, 
               E_total, 
               EDP);
    }

    return 0;
}

float dynamic_energy(float mem_size, float mem_assoc, float mem_accesses) {
    float dynamic_energy;

    dynamic_energy = (((1.43*pow(10,-5)*mem_size) + (1.02*mem_assoc) - (8.63*pow(10,-7)*mem_size*mem_assoc)-0.73)*mem_accesses)*pow(10,-9);

    return dynamic_energy;
}

float static_energy(float mem_size, float sim_time, bool L1, bool L1d) {
    //if L1 is true, then it's L1 cache, else L2 cache, if L1d is true, then it's data cache, else instruction cache
    float static_energy;

    static_energy = ((1.01*pow(10,-7)*L1*L1d +6.25*pow(10,-8)*L1*(!L1d) + 4.29*pow(10,-8)*(!L1))*mem_size*sim_time);

    return static_energy;
}

float total_energy(float CPI, float L1d_access, float L1d_size,float L1d_assoc, float L1i_access, float L1i_size,float L1i_assoc, float L2_access, float L2_size,float L2_assoc, float tsim) {
    float sum_dynamic_energy, sum_static_energy;

    sum_dynamic_energy = dynamic_energy(L1d_size, L1d_assoc, L1d_access) + dynamic_energy(L1i_size, L1i_assoc, L1i_access) + dynamic_energy(L2_size, L2_assoc, L2_access);

    sum_static_energy = static_energy(L1d_size, tsim, true, true) + static_energy(L1i_size, tsim, true, false) + static_energy(L2_size, tsim, false, false);

    return sum_dynamic_energy + sum_static_energy;
}