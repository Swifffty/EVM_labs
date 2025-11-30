import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

FILE_NAME = "access_time_results.csv"

try:
    df = pd.read_csv(FILE_NAME)
except FileNotFoundError:
    print(f"Ошибка: Файл '{FILE_NAME}' не найден. Убедитесь, что он находится в той же папке.")
    exit()

df['Log2_N'] = np.log2(df['N_Elements'])

plt.figure(figsize=(12, 6))

plt.plot(df['Log2_N'], df['Ticks_Direct'], marker='o', linestyle='-', label='Прямой обход (Direct)', color='green')
plt.plot(df['Log2_N'], df['Ticks_Reverse'], marker='s', linestyle='--', label='Обратный обход (Reverse)', color='blue')
plt.plot(df['Log2_N'], df['Ticks_Random'], marker='^', linestyle='-', label='Случайный обход (Random)', color='red')

plt.xlabel('Размер массива N, Log2(N) [100% - RAM]', fontsize=12)
plt.ylabel('Среднее время доступа (Такты процессора)', fontsize=12)
plt.title('Зависимость времени доступа к памяти от размера массива', fontsize=14)

log2_ticks = df['Log2_N'].unique()
N_ticks = [f"{int(2**x)} ({df[df['Log2_N'] == x]['N_Bytes'].iloc[0]/1024/1024:.2f} МБ)"
           for x in log2_ticks]
plt.xticks(log2_ticks, N_ticks, rotation=45, ha='right')
plt.grid(True, which="both", linestyle='--', linewidth=0.5)

plt.legend()
plt.tight_layout()
plt.savefig('access_time_graph.png')
plt.show()
print("\nГрафик сохранен как 'access_time_graph.png'.")
