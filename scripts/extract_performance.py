import pandas as pd
import re
import matplotlib.pyplot as plt


def read_data(path: str):
    with open(path, 'r') as file:
        data = file.read()
    return data


def create_pandas_from_data(path):
    data = read_data(path)
    # Define a regex pattern to match the structure
    pattern = r"VAL:\s*([-.\d]+)Time taken:\s*(\d+)"

    # Find all matches using the regex pattern
    matches = re.findall(pattern, data)

    # Convert the matches into a pandas DataFrame
    df = pd.DataFrame(matches, columns=["VAL", "TimeTaken_ns"])

    # Convert all columns to float for easier analysis
    df = df.astype(float)

    return df

df_tay_orig = create_pandas_from_data('results/performance/TAYLOR_C_ORIGINAL_7pi_inc.txt')
df_tay_orig = df_tay_orig[(df_tay_orig['VAL'] > -3.13) & (df_tay_orig['VAL'] < 3.13)]
print(df_tay_orig)

df_tay_opti_7 = create_pandas_from_data('results/performance/TAYLOR_CPP_OPTIMIZED_7pi_inc.txt')
df_tay_opti_7 = df_tay_opti_7[(df_tay_opti_7['VAL'] > -3.13) & (df_tay_opti_7['VAL'] < 3.13)]
print(df_tay_opti_7)

df_cheb_poly_7 = create_pandas_from_data('results/performance/CHEB_POLY_7pi_inc.txt')
df_cheb_poly_7 = df_cheb_poly_7[(df_cheb_poly_7['VAL'] > -3.13) & (df_cheb_poly_7['VAL'] < 3.13)]
print(df_cheb_poly_7)

df_tay_opti_11 = create_pandas_from_data('results/performance/TAYLOR_CPP_OPTIMIZED_11pi_inc.txt')
df_tay_opti_11 = df_tay_opti_11[(df_tay_opti_11['VAL'] > -3.13) & (df_tay_opti_11['VAL'] < 3.13)]
print(df_tay_opti_11)

df_cheb_poly_11 = create_pandas_from_data('results/performance/CHEB_POLY_11pi_inc.txt')
df_cheb_poly_11 = df_cheb_poly_11[(df_cheb_poly_11['VAL'] > -3.13) & (df_cheb_poly_11['VAL'] < 3.13)]
print(df_cheb_poly_11)

df_tay_opti_15 = create_pandas_from_data('results/performance/TAYLOR_CPP_OPTIMIZED_15pi_inc.txt')
df_tay_opti_15 = df_tay_opti_15[(df_tay_opti_15['VAL'] > -3.13) & (df_tay_opti_15['VAL'] < 3.13)]
print(df_tay_opti_15)

df_cheb_poly_15 = create_pandas_from_data('results/performance/CHEB_POLY_15pi_inc.txt')
df_cheb_poly_15 = df_cheb_poly_15[(df_cheb_poly_15['VAL'] > -3.13) & (df_cheb_poly_15['VAL'] < 3.13)]
print(df_cheb_poly_15)


# Assuming you have the DataFrame stored as `df`
plt.plot(df_tay_orig['VAL'], df_tay_orig['TimeTaken_ns'], label='1000 runs of Taylor original')
plt.plot(df_tay_opti_7['VAL'], df_tay_opti_7['TimeTaken_ns'], label='1000 runs of Taylor optimized degree 7')
plt.plot(df_cheb_poly_7['VAL'], df_cheb_poly_7['TimeTaken_ns'], label='1000 runs of Chebyshev degree 7')
plt.plot(df_tay_opti_11['VAL'], df_tay_opti_11['TimeTaken_ns'], label='1000 runs of Taylor optimized degree 11')
plt.plot(df_cheb_poly_11['VAL'], df_cheb_poly_11['TimeTaken_ns'], label='1000 runs of Chebyshev degree 11')
plt.plot(df_tay_opti_15['VAL'], df_tay_opti_15['TimeTaken_ns'], label='1000 runs of Taylor optimized degree 15')
plt.plot(df_cheb_poly_15['VAL'], df_cheb_poly_15['TimeTaken_ns'], label='1000 runs of Chebyshev degree 15')


plt.xlabel('Value')
plt.ylabel('Runtime')
plt.title('Runtime Performance of sin function')
plt.grid(True)
plt.legend()

# Show the plot
plt.yscale('log')
plt.savefig('plots/performance/inc_rel_pi_001.pdf', format="pdf")
plt.show()
plt.figure()
# plt.plot(df_tay_orig['VAL'], df_tay_orig['TimeTaken_ns'], label='1000 runs of Taylor original')
# plt.plot(df_tay_opti_7['VAL'], df_tay_opti_7['TimeTaken_ns'], label='1000 runs of Taylor optimized degree 7')
# plt.plot(df_cheb_poly_7['VAL'], df_cheb_poly_7['TimeTaken_ns'], label='1000 runs of Chebyshev degree 7')
# plt.plot(df_tay_opti_11['VAL'], df_tay_opti_11['TimeTaken_ns'], label='1000 runs of Taylor optimized degree 11')
# plt.plot(df_cheb_poly_11['VAL'], df_cheb_poly_11['TimeTaken_ns'], label='1000 runs of Chebyshev degree 11')
plt.plot(df_tay_opti_15['VAL'], df_tay_opti_15['TimeTaken_ns'], label='1000 runs of Taylor optimized degree 15')
plt.plot(df_cheb_poly_15['VAL'], df_cheb_poly_15['TimeTaken_ns'], label='1000 runs of Chebyshev degree 15')


plt.xlabel('Value')
plt.ylabel('Runtime')
plt.title('Runtime Performance of sin function')
plt.grid(True)
plt.legend()

# Show the plot
plt.yscale('log')
plt.savefig('plots/performance/inc_rel_pi_tay_vs_cheb.pdf', format="pdf")
plt.show()

plt.figure()
plt.plot(df_tay_orig['VAL'], df_tay_orig['TimeTaken_ns'], label='1000 runs of Taylor original')
plt.plot(df_tay_opti_7['VAL'], df_tay_opti_7['TimeTaken_ns'], label='1000 runs of Taylor optimized degree 7')
# plt.plot(df_cheb_poly_7['VAL'], df_cheb_poly_7['TimeTaken_ns'], label='1000 runs of Chebyshev degree 7')
plt.plot(df_tay_opti_11['VAL'], df_tay_opti_11['TimeTaken_ns'], label='1000 runs of Taylor optimized degree 11')
# plt.plot(df_cheb_poly_11['VAL'], df_cheb_poly_11['TimeTaken_ns'], label='1000 runs of Chebyshev degree 11')
plt.plot(df_tay_opti_15['VAL'], df_tay_opti_15['TimeTaken_ns'], label='1000 runs of Taylor optimized degree 15')
# plt.plot(df_cheb_poly_15['VAL'], df_cheb_poly_15['TimeTaken_ns'], label='1000 runs of Chebyshev degree 15')


plt.xlabel('Value')
plt.ylabel('Runtime')
plt.title('Runtime Performance')
plt.grid(True)
plt.legend()

# Show the plot
plt.yscale('log')
plt.savefig('plots/performance/inc_rel_pi_tay_orig_vs_opt.pdf', format="pdf")
plt.show()

#
