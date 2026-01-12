% Ονόματα Benchmarks από τον νέο πίνακα
bench_names = {'401.bzip2', '429.mcf', '458.sjeng', '470.lbm'};

% Δεδομένα (Ακριβείς τιμές από το image_5a5b47.png)
cpi = [1.68, 1.11, 10.3, 3.50]; 
exec_time = [8.39e-02, 5.54e-02, 5.14e-01, 1.75e-01]; % simSeconds
l1i_misses = [767, 993, 630, 591];
l1d_misses = [745521, 71758, 10523876, 2975097];
l2_misses = [202022, 38806, 5263841, 1488098];

figure('Name', 'Corrected SPEC CPU2006 Correlation Analysis');

% 1. CPI vs L1i Misses
subplot(2,2,1);
scatter(l1i_misses, cpi, 120, 'filled', 'MarkerFaceColor', [0.4 0.8 0.2]);
text(l1i_misses + 20, cpi, bench_names, 'FontSize', 9);
xlabel('L1i Overall Misses'); ylabel('CPI');
title('CPI vs Instruction Cache Misses'); grid on;

% 2. CPI vs L1d Misses
subplot(2,2,2);
scatter(l1d_misses, cpi, 120, 'filled', 'MarkerFaceColor', [0.2 0.6 1.0]);
text(l1d_misses + 400000, cpi, bench_names, 'FontSize', 9);
xlabel('L1d Overall Misses'); ylabel('CPI');
title('CPI vs Data Cache Misses'); grid on;

% 3. CPI vs L2 Misses
subplot(2,2,3);
scatter(l2_misses, cpi, 120, 'filled', 'MarkerFaceColor', [1.0 0.4 0.4]);
text(l2_misses + 200000, cpi, bench_names, 'FontSize', 9);
xlabel('L2 Overall Misses'); ylabel('CPI');
title('CPI vs L2 Cache Misses'); grid on;

% 4. CPI vs Execution Time
subplot(2,2,4);
scatter(exec_time, cpi, 120, 'filled', 'MarkerFaceColor', [0.5 0.2 0.7]);
text(exec_time + 0.02, cpi, bench_names, 'FontSize', 9);
xlabel('Execution Time (seconds)'); ylabel('CPI');
title('CPI vs Total Execution Time'); grid on;

sgtitle('Corrected Correlation of CPI with Memory and Runtime Metrics');
