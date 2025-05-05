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
    pattern = r"VAL:\s*(?P<VAL>[-\d\.e\+]+)\s*SINVAL:\s*(?P<SINVAL>[-\d\.e\+]+)\s*SINVAL\sCUSTOM:\s*(?P<SINVAL_CUSTOM>[-\d\.e\+]+)\s*ABS:\s*(?P<ABS>[-\d\.e\+]+)\s*REL:\s*(?P<REL>[-\d\.e\+]+)"

    # Find all matches using the regex pattern
    matches = re.findall(pattern, data)

    # Convert the matches into a pandas DataFrame
    df = pd.DataFrame(matches, columns=["VAL", "SINVAL", "SINVAL_CUSTOM", "ABS", "REL"])

    # Convert all columns to float for easier analysis
    df = df.astype(float)

    return df
# CHEB_POLY_7four_pi_num_inc
df_tay_orig = create_pandas_from_data('results/accuracy/TAYLOR_C_ORIGINAL_7four_pi_num_inc.txt')
print(df_tay_orig)

df_tay_opti_7 = create_pandas_from_data('results/accuracy/TAYLOR_CPP_OPTIMIZED_7four_pi_num_inc.txt')
print(df_tay_opti_7)

df_cheb_poly_7 = create_pandas_from_data('results/accuracy/CHEB_POLY_7four_pi_num_inc.txt')
print(df_cheb_poly_7)

df_tay_opti_11 = create_pandas_from_data('results/accuracy/TAYLOR_CPP_OPTIMIZED_11four_pi_num_inc.txt')
print(df_tay_opti_11)

df_cheb_poly_11 = create_pandas_from_data('results/accuracy/CHEB_POLY_11four_pi_num_inc.txt')
print(df_cheb_poly_11)

df_tay_opti_15 = create_pandas_from_data('results/accuracy/TAYLOR_CPP_OPTIMIZED_15four_pi_num_inc.txt')
print(df_tay_opti_15)

df_cheb_poly_15 = create_pandas_from_data('results/accuracy/CHEB_POLY_15four_pi_num_inc.txt')
print(df_cheb_poly_15)


# Assuming you have the DataFrame stored as `df`
plt.plot(df_tay_orig['VAL'], df_tay_orig['REL'], label='Relative Error of Taylor original')
plt.plot(df_tay_opti_7['VAL'], df_tay_opti_7['REL'], label='Relative Error of Taylor optimized degree 7')
plt.plot(df_cheb_poly_7['VAL'], df_cheb_poly_7['REL'], label='Relative Error of Chebyshev degree 7')
plt.plot(df_tay_opti_11['VAL'], df_tay_opti_11['REL'], label='Relative Error of Taylor optimized degree 11')
plt.plot(df_cheb_poly_11['VAL'], df_cheb_poly_11['REL'], label='Relative Error of Chebyshev degree 11')
plt.plot(df_tay_opti_15['VAL'], df_tay_opti_15['REL'], label='Relative Error of Taylor optimized degree 15')
plt.plot(df_cheb_poly_15['VAL'], df_cheb_poly_15['REL'], label='Relative Error of Chebyshev degree 15')


plt.xlabel('Value')
plt.ylabel('Relative Error')
plt.title('Dependency of the relative error on the value')
plt.grid(True)
plt.legend()

# Show the plot
plt.yscale('log')
plt.savefig('plots/accuracy/num_rel_four_pi_001.pdf', format="pdf")
plt.show()


plt.figure()
# Assuming you have the DataFrame stored as `df`
plt.plot(df_tay_orig['VAL'], df_tay_orig['ABS'], label='Absolute Error of Taylor original')
plt.plot(df_tay_opti_7['VAL'], df_tay_opti_7['ABS'], label='Absolute Error of Taylor optimized degree 7')
plt.plot(df_cheb_poly_7['VAL'], df_cheb_poly_7['ABS'], label='Absolute Error of Chebyshev degree 7')
plt.plot(df_tay_opti_11['VAL'], df_tay_opti_11['ABS'], label='Absolute Error of Taylor optimized degree 11')
plt.plot(df_cheb_poly_11['VAL'], df_cheb_poly_11['ABS'], label='Absolute Error of Chebyshev degree 11')
plt.plot(df_tay_opti_15['VAL'], df_tay_opti_15['ABS'], label='Absolute Error of Taylor optimized degree 15')
plt.plot(df_cheb_poly_15['VAL'], df_cheb_poly_15['ABS'], label='Absolute Error of Chebyshev degree 15')

plt.xlabel('Value')
plt.ylabel('Absolute Error')
plt.title('Dependency of the absolute error on the value')
plt.grid(True)
plt.legend()

# Show the plot
plt.yscale('log')
plt.savefig('plots/accuracy/num_abs_four_pi_001.pdf', format="pdf")
plt.show()
