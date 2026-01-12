% MATLAB Code to plot CPI Analysis based on CPI_Final_Results.txt

clear; clc; close all;

% Δεδομένα όπως προέκυψαν από το αρχείο (Hardcoded για ευκολία)
% Μορφή: [L1_Size(kB), L1_Assoc, L2_Size(MB), CPI]

% bzip2 Data
bzip2_data = [
    64, 4, 2, 1.660740;
    64, 4, 4, 1.635076;
    64, 8, 2, 1.654375;
    64, 8, 4, 1.628651;
    128, 4, 2, 1.637125;
    128, 4, 4, 1.611404;
    128, 8, 2, 1.632979;
    128, 8, 4, 1.607446;
];

% mcf Data
mcf_data = [
    64, 4, 2, 1.107753;
    64, 4, 4, 1.107436;
    64, 8, 2, 1.107701;
    64, 8, 4, 1.107384;
    128, 4, 2, 1.107390;
    128, 4, 4, 1.107072;
    128, 8, 2, 1.107389;
    128, 8, 4, 1.107072;
];

% sjeng Data
sjeng_data = [
    64, 4, 2, 9.875443;
    64, 4, 4, 9.871248;
    64, 8, 2, 9.875515;
    64, 8, 4, 9.871093;
    128, 4, 2, 9.875216;
    128, 4, 4, 9.871404;
    128, 8, 2, 9.875216;
    128, 8, 4, 9.871404;
];

% lbm Data
lbm_data = [
    64, 4, 2, 3.435670;
    64, 4, 4, 3.431821;
    64, 8, 2, 3.435670;
    64, 8, 4, 3.431933;
    128, 4, 2, 3.435670;
    128, 4, 4, 3.431933;
    128, 8, 2, 3.435670;
    128, 8, 4, 3.431933;
];

% Ονόματα Benchmarks και Labels
benchmarks = {'bzip2', 'mcf', 'sjeng', 'lbm'};
all_data = {bzip2_data, mcf_data, sjeng_data, lbm_data};

% Δημιουργία Labels για τον άξονα X
labels = {};
for i = 1:size(bzip2_data, 1)
    labels{i} = sprintf('L1:%dk-a%d L2:%dM', ...
        bzip2_data(i,1), bzip2_data(i,2), bzip2_data(i,3));
end

% Σχεδίαση
figure('Position', [100, 100, 1200, 800]);

for i = 1:4
    subplot(2, 2, i);
    current_data = all_data{i};
    cpi_vals = current_data(:, 4);
    
    b = bar(cpi_vals);
    b.FaceColor = 'flat';
    b.CData = [0 0.4470 0.7410]; % Χρώμα μπλε
    
    title(['Benchmark: ' benchmarks{i}]);
    ylabel('CPI (Cycles Per Instruction)');
    set(gca, 'XTickLabel', labels, 'XTick', 1:length(labels));
    xtickangle(45);
    grid on;
    
    % Zoom στον άξονα Y για να φανούν οι μικρές διαφορές
    ylim([min(cpi_vals)*0.98, max(cpi_vals)*1.01]);
    
    % Εμφάνιση τιμών πάνω από τις μπάρες
    text(1:length(cpi_vals), cpi_vals, num2str(cpi_vals, '%.4f'), ...
        'vert','bottom','horiz','center', 'FontSize', 8);
end

sgtitle('Ανάλυση CPI ανά Benchmark και Configuration');
