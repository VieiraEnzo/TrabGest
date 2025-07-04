import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

def plot_and_save_umatrix():
    try:
        script_dir = Path(__file__).parent.resolve()
        data_path = script_dir.parent.parent / 'results' / 'kohonen_u_matrix.csv'
        
        output_path = script_dir / 'kohonen_u_matrix.png'

        print(f"1. Lendo os dados da U-Matrix de: {data_path}")
        u_matrix = np.loadtxt(data_path, delimiter=',')

        print("2. Gerando o heatmap da U-Matrix...")
        fig, ax = plt.subplots(figsize=(10, 8))
        im = ax.imshow(u_matrix, cmap='viridis', interpolation='none')

        cbar = fig.colorbar(im)
        cbar.set_label('Distância Média dos Vizinhos', rotation=270, labelpad=15, fontsize=12)

        ax.set_title('U-Matrix do SOM Treinado', fontsize=16)
        ax.set_xlabel('Neurônio na Matriz (Coordenada X)', fontsize=12)
        ax.set_ylabel('Neurônio na Matriz (Coordenada Y)', fontsize=12)

        ax.set_xticks(np.arange(u_matrix.shape[1]))
        ax.set_yticks(np.arange(u_matrix.shape[0]))
        ax.set_xticks(np.arange(-.5, u_matrix.shape[1], 2), minor=True)
        ax.set_yticks(np.arange(-.5, u_matrix.shape[0], 2), minor=True)
        ax.grid(which='minor', color='w', linestyle='-', linewidth=0.5, alpha=0.3);

        output_path.parent.mkdir(parents=True, exist_ok=True);

        plt.savefig(output_path, dpi=300, bbox_inches='tight');
        
        print(f"\nSucesso! Imagem da U-Matrix salva em: {output_path}");

    except FileNotFoundError:
        print(f"\n--- ERRO ---");
        print(f"Arquivo de dados não encontrado em '{data_path}'.");
    except Exception as e:
        print(f"Ocorreu um erro inesperado: {e}");

if __name__ == '__main__':
    plot_and_save_umatrix();