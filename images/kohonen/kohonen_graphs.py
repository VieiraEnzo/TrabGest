import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

def generate_dual_axis_plot(df, parameter_col, output_dir, **kwargs):
    if parameter_col not in df.columns or df[parameter_col].nunique() <= 1:
        print(f"Pulando gráfico para '{parameter_col}': dados insuficientes.\n")
        return
    required_errors = ['quantizationError', 'topographicError']
    if not all(err in df.columns for err in required_errors):
        print(f"Pulando gráfico para '{parameter_col}': colunas de erro não encontradas.\n")
        return

    print(f"Gerando gráfico para '{parameter_col}'...")

    df_copy = df.copy()

    df_copy[parameter_col] = pd.to_numeric(df_copy[parameter_col])

    qe_analysis = df_copy.groupby(parameter_col)['quantizationError'].mean().sort_index()
    te_analysis = df_copy.groupby(parameter_col)['topographicError'].mean().sort_index()
    
    fig, ax1 = plt.subplots(figsize=(12, 7))

    color1 = 'tab:blue'
    ax1.set_xlabel(kwargs.get('x_label', 'Parâmetro'), fontsize=12)
    ax1.set_ylabel('Erro Médio de Quantização', color=color1, fontsize=12)
    line1 = ax1.plot(qe_analysis.index, qe_analysis.values, color=color1, marker='o', linestyle='-', label='Erro de Quantização')
    ax1.tick_params(axis='y', labelcolor=color1)

    ax2 = ax1.twinx()
    color2 = 'tab:red'
    ax2.set_ylabel('Erro Médio Topográfico', color=color2, fontsize=12)
    line2 = ax2.plot(te_analysis.index, te_analysis.values, color=color2, marker='o', linestyle='-', label='Erro Topográfico')
    ax2.tick_params(axis='y', labelcolor=color2)

    plt.title(kwargs.get('title', 'Gráfico de Erro'), fontsize=16)
    ax1.grid(True, which='both', linestyle='--', linewidth=0.5)

    lines = line1 + line2
    labels = [l.get_label() for l in lines]
    ax1.legend(lines, labels, loc='best', fontsize=12)

    fig.tight_layout()
    save_path = output_dir / kwargs.get('filename', 'dual_axis_plot.png')
    plt.savefig(save_path)
    plt.close()
    print(f"Gráfico salvo em: {save_path}\n")

def analyze_kohonen_results(csv_path, output_dir):
    print(f"Tentando carregar dados de: {csv_path}")
    try:
        df = pd.read_csv(csv_path)
        df.columns = df.columns.str.strip()
        print("Dados carregados com sucesso.")
    except FileNotFoundError:
        print(f"--- ERRO: Arquivo não encontrado em {csv_path}.")
        return
    except Exception as e:
        print(f"Ocorreu um erro ao ler o arquivo: {e}")
        return

    plot_configs = [
        {'parameter_col': 'sigma', 'title': 'Análise de Erros vs. Raio de Vizinhança (σ)', 'x_label': 'Sigma (σ)', 'filename': 'kohonen_dual_error_vs_sigma.png'},
        {'parameter_col': 'epocas', 'title': 'Análise de Erros vs. Número de Épocas', 'x_label': 'Número de Épocas', 'filename': 'kohonen_dual_error_vs_epocas.png'},
        {'parameter_col': 'lr', 'title': 'Análise de Erros vs. Taxa de Aprendizagem (lr)', 'x_label': 'Taxa de Aprendizagem (lr)', 'filename': 'kohonen_dual_error_vs_lr.png'},
        {'parameter_col': 'Tmatriz', 'title': 'Análise de Erros vs. Lado da Matriz', 'x_label': 'Lado da Matriz', 'filename': 'kohonen_dual_error_vs_tmatriz.png'}
    ]

    for config in plot_configs:
        generate_dual_axis_plot(
            df, 
            output_dir=output_dir,
            **config
        )

if __name__ == '__main__':
    script_dir = Path(__file__).parent
    csv_file_path = script_dir.parent.parent / 'results' / 'Kohonen.csv'
    output_directory = script_dir
    
    output_directory.mkdir(parents=True, exist_ok=True)

    analyze_kohonen_results(csv_path=csv_file_path, output_dir=output_directory)
