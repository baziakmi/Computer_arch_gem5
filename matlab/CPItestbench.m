% MATLAB Code for CPI Analysis - Combined Dataset (80 Configurations)
clear; clc; close all;

% Φόρτωση των δεδομένων από το FINAL_CPI.txt
% Σημείωση: Ο κώδικας υποθέτει ότι το αρχείο βρίσκεται στο ίδιο folder
data_raw = readtable('FINAL_CPI.txt', 'Delimiter', ' ', 'MultipleDelimsAsOne', true);

% Ονόματα Benchmarks
bench_names = {'bzip2', 'mcf', 'sjeng', 'lbm'};

figure('Position', [50, 50, 1400, 900]);

for i = 1:length(bench_names)
    % Φιλτράρισμα δεδομένων για το συγκεκριμένο benchmark
    idx = contains(data_raw.Benchmarks, bench_names{i});
    current_table = data_raw(idx, :);
    
    % Ταξινόμηση για καλύτερη οπτικοποίηση (προαιρετικά)
    cpi_vals = current_table.system_cpu_cpi;
    labels = current_table.Benchmarks;
    
    % Καθαρισμός ετικετών (αφαίρεση του ονόματος του benchmark για οικονομία χώρου)
    clean_labels = strrep(labels, [bench_names{i} '_'], '');
    
    subplot(2, 2, i);
    b = bar(cpi_vals);
    b.FaceColor = 'flat';
    
    % Χρωματισμός ανάλογα με το CPI (πιο σκούρο μπλε για υψηλό CPI)
    b.CData = repmat([0.2 0.4 0.8], length(cpi_vals), 1); 
    
    title(['Benchmark: ' bench_names{i}], 'FontSize', 12, 'FontWeight', 'bold');
    ylabel('CPI (Cycles Per Instruction)');
    grid on;
    
    % Ρυθμίσεις αξόνων
    set(gca, 'XTick', 1:length(clean_labels), 'XTickLabel', clean_labels);
    xtickangle(90); % Κάθετες ετικέτες λόγω μεγάλου πλήθους δεδομένων
    set(gca, 'FontSize', 7);
    
    % Δυναμικό Zoom στον άξονα Y για ανάδειξη διαφορών
    ymin = min(cpi_vals);
    ymax = max(cpi_vals);
    ylim([ymin * 0.995, ymax * 1.005]);
    
    % Προσθήκη τιμών μόνο αν είναι ευανάγνωστες (στα 4 benchmarks με 20 configs έκαστο)
    if length(cpi_vals) <= 20
        text(1:length(cpi_vals), cpi_vals, num2str(cpi_vals, '%.3f'), ...
            'vert','bottom','horiz','center', 'FontSize', 6, 'Rotation', 90);
    end
end

sgtitle('Συνολική Ανάλυση CPI: 80 Configurations ανά Benchmark', 'FontSize', 16);
