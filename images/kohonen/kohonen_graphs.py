import pandas as pd
import matplotlib.pyplot as plt
import os
from pathlib import Path

def analyze_kohonen_results(csv_path, output_dir):
    print(f"Attempting to load data from: {csv_path}")

    try:
        df = pd.read_csv(csv_path, dtype={'Tmatriz': str})
        
        df.columns = df.columns.str.strip()
        print("Data loaded successfully.")
    except FileNotFoundError:
        print(f"--- ERROR: File Not Found ---")
        print(f"The file was not found at the path: {csv_path}")
        print("Please check the path calculation in the main block of the script.")
        return
    except Exception as e:
        print(f"An unexpected error occurred while reading the file: {e}")
        return

    if 'sigma' in df.columns and df['sigma'].nunique() > 1:
        print("Generating plot for Quantization Error vs. Sigma...")
        
        df['sigma'] = pd.to_numeric(df['sigma'])
        sigma_analysis = df.groupby('sigma')['quantizationError'].mean().sort_index()
        
        best_sigma = sigma_analysis.idxmin()
        min_error_sigma = sigma_analysis.min()
        
        plt.figure(figsize=(10, 6))
        sigma_analysis.plot(marker='o', linestyle='-', color='b', label='Erro Médio de Quantização')
        
        plt.plot(best_sigma, min_error_sigma, 'r*', markersize=15, label=f'Ótimo: σ={best_sigma:.2f}')
        
        plt.title('Erro de Quantização vs. Raio de Vizinhança (σ)', fontsize=16)
        plt.xlabel('Sigma (σ)', fontsize=12)
        plt.ylabel('Erro Médio de Quantização', fontsize=12)
        plt.grid(True, which='both', linestyle='--', linewidth=0.5)
        plt.legend(fontsize=12)
        
        save_path = output_dir / 'kohonen_qe_sigma.png'
        plt.savefig(save_path)
        plt.close()
        print(f"Plot saved to: {save_path}\n")
    else:
        print(f"Skipping Sigma plot: Found {df.get('sigma', pd.Series()).nunique()} unique 'sigma' value(s). Need more than 1 to plot.\n")

    if 'epocas' in df.columns and df['epocas'].nunique() > 1:
        print("Generating plot for Quantization Error vs. Epochs...")
        
        df['epocas'] = pd.to_numeric(df['epocas'])
        epoch_analysis = df.groupby('epocas')['quantizationError'].mean().sort_index()
        best_epoch = epoch_analysis.idxmin()
        min_error_epoch = epoch_analysis.min()
        
        plt.figure(figsize=(10, 6))
        epoch_analysis.plot(marker='o', linestyle='-', color='g')
        
        plt.plot(best_epoch, min_error_epoch, 'r*', markersize=15, label=f'Ótimo: {int(best_epoch)} épocas')

        plt.title('Erro de Quantização vs. Número de Épocas', fontsize=16)
        plt.xlabel('Número de Épocas', fontsize=12)
        plt.ylabel('Erro Médio de Quantização', fontsize=12)
        plt.grid(True, which='both', linestyle='--', linewidth=0.5)
        plt.legend(fontsize=12)
        
        save_path = output_dir / 'kohonen_qe_epocas.png'
        plt.savefig(save_path)
        plt.close()
        print(f"Plot saved to: {save_path}\n")
    else:
        print(f"Skipping Epochs plot: Found {df.get('epocas', pd.Series()).nunique()} unique 'epocas' value(s). Need more than 1 to plot.\n")

    if 'lr' in df.columns and df['lr'].nunique() > 1:
        print("Generating plot for Quantization Error vs. Learning Rate (lr)...")
        
        df['lr'] = pd.to_numeric(df['lr'])
        lr_analysis = df.groupby('lr')['quantizationError'].mean().sort_index()
        best_lr = lr_analysis.idxmin()
        min_error_lr = lr_analysis.min()
        
        plt.figure(figsize=(10, 6))
        lr_analysis.plot(marker='o', linestyle='-', color='orange')
        
        plt.plot(best_lr, min_error_lr, 'r*', markersize=15, label=f'Ótimo: lr={best_lr:.5f}')

        plt.title('Erro de Quantização vs. Taxa de Aprendizagem (lr)', fontsize=16)
        plt.xlabel('Taxa de Aprendizagem (lr)', fontsize=12)
        plt.ylabel('Erro Médio de Quantização', fontsize=12)
        plt.grid(True, which='both', linestyle='--', linewidth=0.5)
        plt.legend(fontsize=12)
        
        save_path = output_dir / 'kohonen_qe_lr.png'
        plt.savefig(save_path)
        plt.close()
        print(f"Plot saved to: {save_path}\n")
    else:
        print(f"Skipping Learning Rate plot: Found {df.get('lr', pd.Series()).nunique()} unique 'lr' value(s). Need more than 1 to plot.\n")

    if 'Tmatriz' in df.columns and df['Tmatriz'].nunique() > 1:
        print("Generating plot for Quantization Error vs. Lado da Matriz...")
        
        try:
            df['lado_matriz'] = pd.to_numeric(df['Tmatriz'].apply(lambda x: x.lower().split('x')[0]))
            
            grid_analysis = df.groupby('lado_matriz')['quantizationError'].mean().sort_index()
            
            best_side = grid_analysis.idxmin()
            min_error_grid = grid_analysis.min()
            
            total_neurons_optimal = best_side * best_side

            plt.figure(figsize=(10, 6))
            grid_analysis.plot(marker='o', linestyle='-', color='purple')

            plt.plot(best_side, min_error_grid, 'r*', markersize=15, 
                     label=f'Ótimo: Lado {best_side} ({int(total_neurons_optimal)} neurônios)')

            plt.title('Erro de Quantização vs. Lado da Matriz', fontsize=16)
            plt.xlabel('Lado da Matriz', fontsize=12)
            plt.ylabel('Erro Médio de Quantização', fontsize=12)

            plt.grid(True, which='both', linestyle='--', linewidth=0.5)
            plt.legend(fontsize=12)
            
            save_path = output_dir / 'kohonen_qe_tmatriz.png'
            plt.savefig(save_path)
            plt.close()
            print(f"Plot saved to: {save_path}\n")
            
        except Exception as e:
            print(f"--- ERROR ---")
            print(f"Could not parse 'Tmatriz' column. Ensure values are like '5' or '5x5'. Error: {e}\n")
    else:
        print(f"Skipping Lado da Matriz plot: Found {df.get('Tmatriz', pd.Series()).nunique()} unique 'Tmatriz' value(s). Need more than 1 to plot.\n")

if __name__ == '__main__':
    script_dir = Path(__file__).parent
    
    csv_file_path = script_dir / '../../results/Kohonen.csv'
    
    analyze_kohonen_results(csv_path=csv_file_path, output_dir=script_dir)