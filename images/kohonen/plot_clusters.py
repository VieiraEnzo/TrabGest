

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path


SCRIPT_DIR = Path(__file__).parent
PROJECT_ROOT = SCRIPT_DIR.parent.parent
RESULTS_DIR = PROJECT_ROOT / "results"

def generate_cluster_plots(csv_path):
    
    print(f"1. Loading data from: {csv_path}")
    try:
        df = pd.read_csv(csv_path)
    except FileNotFoundError:
        print(f"--- ERROR: File not found at {csv_path}. Please check the path.")
        print("   - Please run the C++ simulation first to generate the required data.")
        return

    
    hyperparameters = df[['epochs', 'lr', 'sigma', 'Tmatriz']].drop_duplicates()

    print(f"Found {len(hyperparameters)} unique hyperparameter combinations to plot.")

    
    output_dir = SCRIPT_DIR / "cluster_plots"
    output_dir.mkdir(exist_ok=True)
    print(f"Plots will be saved in: {output_dir}")

    
    for index, row in hyperparameters.iterrows():
        epoch = row['epochs']
        lr = row['lr']
        sigma = row['sigma']
        t_matriz = row['Tmatriz']

        print(f"\n2. Generating plot for: epochs={epoch}, lr={lr}, sigma={sigma}, T={t_matriz}x{t_matriz}...")

        
        plot_data = df[
            (df['epochs'] == epoch) &
            (df['lr'] == lr) &
            (df['sigma'] == sigma) &
            (df['Tmatriz'] == t_matriz)
        ]

        if plot_data.empty:
            print("   -> No data found for this combination. Skipping.")
            continue

        fig, ax = plt.subplots(figsize=(8, 7))

        
        unique_bmus = sorted(plot_data['bmu_index'].unique())
        num_clusters = len(unique_bmus)
        cmap = plt.get_cmap('viridis', num_clusters)

        
        scatter = ax.scatter(
            plot_data['attr1'],
            plot_data['attr2'],
            c=plot_data['bmu_index'],
            
            alpha=0.8,
            
            vmin=min(unique_bmus) - 0.5,
            vmax=max(unique_bmus) + 0.5
        )

        
        cbar = fig.colorbar(scatter, ax=ax)

        
        if num_clusters > 1:
            tick_step = max(1, num_clusters // 15)
            ticks = unique_bmus[::tick_step]
            if unique_bmus[-1] not in ticks:
                ticks.append(unique_bmus[-1])
            cbar.set_ticks(ticks)
            cbar.set_ticklabels([int(t) for t in ticks])

        cbar.set_label('BMU Index (Cluster)', rotation=270, labelpad=15)

        
        ax.set_title(f'BMU Clusters\nEpochs={epoch}, LR={lr}, Sigma={sigma}, T={t_matriz}x{t_matriz}')
        ax.set_xlabel('Attribute 1')
        ax.set_ylabel('Attribute 2')
        ax.grid(True, linestyle='--', alpha=0.6)

        
        output_filename = f"bmu_cluster_t{int(t_matriz)}_e{int(epoch)}_lr{lr}_s{sigma}.png"
        output_path = output_dir / output_filename
        plt.savefig(output_path, dpi=150, bbox_inches='tight')
        plt.close(fig)
        print(f"   -> Plot saved as: {output_path}")

if __name__ == '__main__':
    csv_file_path = RESULTS_DIR / "kohonen_clusters.csv"
    generate_cluster_plots(csv_file_path)

