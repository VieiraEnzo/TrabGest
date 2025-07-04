import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path

# --- Path Setup ---
# Get the directory where this script is located.
SCRIPT_DIR = Path(__file__).parent
# The project root is assumed to be two levels up from the script directory.
PROJECT_ROOT = SCRIPT_DIR.parent.parent


def analyze_umatrix_grids(csv_path, t_matriz):
    """
    Loads a U-Matrix CSV file and generates a grid of visualizations
    for each 'epochs' value.

    Args:
        csv_path (Path): The path to the CSV file.
        t_matriz (int): The dimension of the SOM (e.g., 10 for a 10x10 map).
    """
    print(f"1. Loading data from: {csv_path}")
    try:
        df = pd.read_csv(csv_path)
    except FileNotFoundError:
        print(f"--- ERROR: File not found at {csv_path}. Please check the path.")
        return

    epoch_values = sorted(df['epochs'].unique())
    lr_values = sorted(df['lr'].unique())
    sigma_values = sorted(df['sigma'].unique())

    print(f"Parameters found:")
    print(f"  - Epochs: {epoch_values}")
    print(f"  - Learning Rates: {lr_values}")
    print(f"  - Sigmas: {sigma_values}")

    umatrix_cols = [f'umatrix_{r}_{c}' for r in range(t_matriz) for c in range(t_matriz)]

    for epoch in epoch_values:
        print(f"\n2. Generating grid for epochs = {epoch}...")

        fig, axes = plt.subplots(
            nrows=len(lr_values), 
            ncols=len(sigma_values), 
            figsize=(len(sigma_values) * 3.5, len(lr_values) * 3.5),
            squeeze=False
        )
        
        fig.suptitle(f'U-Matrices for {epoch} Epochs (t_matriz={t_matriz}x{t_matriz})', fontsize=20, y=1.02)

        for i, lr in enumerate(lr_values):
            for j, sigma in enumerate(sigma_values):
                ax = axes[i, j]

                row_data = df[
                    (df['epochs'] == epoch) & 
                    (df['lr'] == lr) & 
                    (df['sigma'] == sigma)
                ]

                if row_data.empty:
                    ax.text(0.5, 0.5, "No Data", ha='center', va='center')
                    ax.set_xticks([])
                    ax.set_yticks([])
                    continue

                umatrix_flat = row_data.iloc[0][umatrix_cols].to_numpy()
                umatrix_2d = umatrix_flat.reshape((t_matriz, t_matriz))

                # Plot the heatmap. Remember: Yellow = High Distance (Boundary), Purple = Low Distance (Cluster)
                im = ax.imshow(umatrix_2d, cmap='viridis', interpolation='nearest')

                if i == 0:
                    ax.set_title(f'σ = {sigma}', fontsize=14)
                if j == 0:
                    ax.set_ylabel(f'lr = {lr}', fontsize=14, rotation=90, labelpad=20)
                
                ax.set_xticks([])
                ax.set_yticks([])

        plt.tight_layout(rect=[0, 0, 1, 0.98])
        
        output_filename = f"kohonen_umatrix_grid_t{t_matriz}_epochs_{epoch}.png"
        output_dir = SCRIPT_DIR / "u_matrix_plots"
        output_dir.mkdir(parents=True, exist_ok=True)
        output_path = output_dir / output_filename
        plt.savefig(output_path, dpi=150, bbox_inches='tight')
        plt.close(fig)
        print(f"   -> Grid saved as: {output_path}")


if __name__ == '__main__':
    results_dir = PROJECT_ROOT / "results"
    
    umatrix_files = sorted(list(results_dir.glob("kohonen_umatrix_t*.csv")))
    
    if not umatrix_files:
        print("--- ERROR: No U-Matrix CSV files found in the 'results' directory.")
        print("   - Looked for files matching: 'kohonen_umatrix_t*.csv'")
        print(f"   - Searched in: {results_dir}")
        print("   - Please run the C++ simulation first to generate the required data.")
    else:
        print(f"Found {len(umatrix_files)} U-Matrix CSV files to process.")
        
        for csv_path in umatrix_files:
            try:
                filename_stem = csv_path.stem
                size_str = filename_stem.split('_t')[-1]
                t_matriz_size = int(size_str)
                
                print("\n" + "="*60)
                print(f"Processing file: {csv_path.name}")
                print(f"Detected SOM Size: {t_matriz_size}x{t_matriz_size}")
                print("="*60)
                
                analyze_umatrix_grids(csv_path, t_matriz_size)
                
            except (ValueError, IndexError):
                print(f"\n--- WARNING: Could not parse t_matriz size from filename: {csv_path.name}. Skipping.")