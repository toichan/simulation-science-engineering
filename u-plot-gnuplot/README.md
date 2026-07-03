# u-plot-gnuplot

This project is designed to compute and visualize the values of the function `u()` defined in the C program located in `src/Q5.c`. The function calculates values based on specified parameters and outputs them for plotting.

## Project Structure

- `src/Q5.c`: Contains the source code for the C program that defines the function `u()` and computes its values.
- `scripts/build_and_run.sh`: A shell script to compile the C program and run it, redirecting the output to `data/u_values.dat`.
- `scripts/plot_u.gnu`: A gnuplot script that reads data from `u_values.dat` and plots the values of the function `u()`.
- `data/u_values.dat`: A data file that stores the output of the C program, containing the computed values of the function `u()`.
- `Makefile`: A Makefile to manage the build process, making it easy to compile and run the C program and generate the data file.
- `.gitignore`: Specifies files and directories to be ignored by Git, typically including compiled binaries and temporary files.
  
## Usage

1. **Build and Run the Program**: You can compile and run the C program using the provided shell script:
   ```bash
   ./scripts/build_and_run.sh
   ```
   This will generate the `data/u_values.dat` file containing the output of the function `u()`.

2. **Plot the Results**: After generating the data file, you can plot the results using gnuplot:
   ```bash
   gnuplot scripts/plot_u.gnu
   ```

## Dependencies

- GCC (GNU Compiler Collection) for compiling the C program.
- Gnuplot for plotting the data.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.