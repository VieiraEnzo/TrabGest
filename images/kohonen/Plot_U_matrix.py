import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

def plot_and_save_umatrix():
    """
    Lê os dados da U-Matrix de um arquivo CSV e gera um heatmap pixelado,
    salvando-o como uma imagem PNG.
    """
    try:
        # Define os caminhos de forma robusta a partir da localização do script
        script_dir = Path(__file__).parent.resolve()
        # O arquivo de dados está no diretório 'results' na raiz do projeto
        data_path = script_dir.parent.parent / 'results' / 'kohonen_u_matrix.csv'
        
        # A imagem de saída será salva no mesmo diretório do script
        output_path = script_dir / 'kohonen_u_matrix.png'

        print(f"1. Lendo os dados da U-Matrix de: {data_path}")
        u_matrix = np.loadtxt(data_path, delimiter=',')

        print("2. Gerando o heatmap PIXELADO da U-Matrix...")
        fig, ax = plt.subplots(figsize=(10, 8))
        im = ax.imshow(u_matrix, cmap='viridis', interpolation='none')

        # Adiciona a barra de cores para dar escala aos valores originais
        cbar = fig.colorbar(im)
        cbar.set_label('Distância Média dos Vizinhos', rotation=270, labelpad=15, fontsize=12)

        ax.set_title('U-Matrix do SOM Treinado', fontsize=16)
        ax.set_xlabel('Neurônio na Matriz (Coordenada X)', fontsize=12)
        ax.set_ylabel('Neurônio na Matriz (Coordenada Y)', fontsize=12)

        # Ajusta os ticks para corresponderem ao tamanho da matriz 10x10
        ax.set_xticks(np.arange(u_matrix.shape[1]))
        ax.set_yticks(np.arange(u_matrix.shape[0]))
        ax.set_xticks(np.arange(-.5, u_matrix.shape[1], 2), minor=True)
        ax.set_yticks(np.arange(-.5, u_matrix.shape[0], 2), minor=True)
        ax.grid(which='minor', color='w', linestyle='-', linewidth=0.5, alpha=0.3)

        # Garante que o diretório de saída exista
        output_path.parent.mkdir(parents=True, exist_ok=True)

        # Salva a figura
        plt.savefig(output_path, dpi=300, bbox_inches='tight')
        
        print(f"\nSucesso! Imagem da U-Matrix salva em: {output_path}")

    except FileNotFoundError:
        print(f"\n--- ERRO ---")
        print(f"Arquivo de dados não encontrado em '{data_path}'.")
        print("Você executou o programa C++ 'gerar_umatrix' primeiro?")
    except Exception as e:
        print(f"Ocorreu um erro inesperado: {e}")

if __name__ == '__main__':
    plot_and_save_umatrix()