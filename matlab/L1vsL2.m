% Ονόματα Benchmarks από τον σωστό πίνακα
bench_names = {'401.bzip2', '429.mcf', '458.sjeng', '470.lbm'};

% Δεδομένα (Ακριβείς τιμές από το image_5a5b47.png)
l1i_misses = [767, 993, 630, 591];
l1d_misses = [745521, 71758, 10523876, 2975097];
l2_misses = [202022, 38806, 5263841, 1488098];

figure('Name', 'Cache Miss Correlation Analysis');

% 1. Συσχέτιση L1d Misses vs L2 Misses (Data Path)
subplot(2,1,1);
scatter(l1d_misses, l2_misses, 150, 'filled', 'MarkerFaceColor', [0.85 0.33 0.10]);
text(l1d_misses + 400000, l2_misses, bench_names, 'FontSize', 10, 'FontWeight', 'bold');
xlabel('L1d Overall Misses'); 
ylabel('L2 Overall Misses');
title('Correlation: L1 Data Cache Misses vs L2 Misses');
grid on;

% 2. Συσχέτιση L1i Misses vs L2 Misses (Instruction Path)
subplot(2,1,2);
scatter(l1i_misses, l2_misses, 150, 'filled', 'MarkerFaceColor', [0.47 0.67 0.19]);
text(l1i_misses + 15, l2_misses, bench_names, 'FontSize', 10, 'FontWeight', 'bold');
xlabel('L1i Overall Misses'); 
ylabel('L2 Overall Misses');
title('Correlation: L1 Instruction Cache Misses vs L2 Misses');
grid on;

% Υπολογισμός συντελεστών συσχέτισης (Correlation Coefficients)
R_d_l2 = corrcoef(l1d_misses, l2_misses);
R_i_l2 = corrcoef(l1i_misses, l2_misses);

fprintf('Συσχέτιση L1d - L2: %.4f\n', R_d_l2(1,2));
fprintf('Συσχέτιση L1i - L2: %.4f\n', R_i_l2(1,2));

sgtitle('Inter-level Cache Miss Correlation (Corrected Data)');
