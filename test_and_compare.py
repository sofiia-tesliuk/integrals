import sys
import glob
import subprocess
import math


def clear_output(filenames):
    for filename in filenames:
        f = open(filename, 'w')
        f.close()


def parse_config(config_filename):
    with open(config_filename, 'r') as file:
        content = file.readlines()
        try:
            abs_err = float(content[0].split('#')[0])
            rel_err = float(content[1].split('#')[0])
            return abs_err, rel_err
        except (TypeError, ValueError, IndexError):
            print('Something wrong with config_filename: {}'.format(config_filename))


def change_num_of_threads_in_config(config_filename, new_value):
    with open(config_filename, 'r') as file:
        content = file.readlines()

    try:
        line = content[2].split('#')
        content[2] = '{}\t#{}'.format(new_value, line[-1])
    except IndexError:
        print('Something wrong with config_filename: {}'.format(config_filename))
        return None

    with open(config_filename, 'w') as file:
        file.writelines(content)


def find_path_to_file(filename):
    try:
        return glob.glob('**/{}'.format(filename))[0][:-len(filename)]
    except IndexError:
        print('Have you compiled {}.cpp?. Cannot find {} in this project.'.format(filename, filename))


def run_program(program_name, program_path, arguments, out_filename, cerr_filename):
    out = open(out_filename, 'a')
    cerr = open(cerr_filename, 'a')
    r = subprocess.run(['./{}{}'.format(program_path, program_name)] + arguments, stdout=out, stderr=cerr)
    out.close()
    cerr.close()
    if isinstance(r, subprocess.CalledProcessError):
        print('Something gone wrong with running `{} {}`'.format('{}{}'.format(program_path, program_name),
                                                                 ' '.join(arguments)))
        return False
    else:
        return True


def parse_and_check_out(filename_out, abs_err, rel_err):
    min_time = float('inf')
    with open(filename_out, 'r') as file:
        content = file.readlines()

    try:
        for i in range(0, len(content), 3):
            errors = content[i + 1].split('|')[-1].split(':')
            cur_abs_err = float(errors[0])
            cur_rel_err = float(errors[1])
            if (cur_rel_err > rel_err) and (cur_abs_err > abs_err):
                print('abs_err: {} > {}\trel_err: {} > {}'.format(cur_abs_err, abs_err, cur_rel_err, rel_err))
                return None
            cur_time = int(content[i + 2].split()[-1])
            if cur_time < min_time:
                min_time = cur_time
    except (TypeError, ValueError, IndexError) as err:
        print(err)
        print('Something wrong with this file: {}. Result format is not supported'.format(filename_out))

    return min_time


def plot_results(consistent_res, parallel_res_i, parallel_res_10_i):
    import matplotlib.pyplot as plt

    x = list(range(1, 5))
    y1 = parallel_res_i
    cons_y = [consistent_res] * len(x)
    plt.xlim((0, 5))
    plt.plot(x, y1)
    plt.plot(x, cons_y, color='green')
    plt.xlabel('Number of threads')
    plt.ylabel('Total time (microseconds) of processing')
    plt.savefig('parl_res_i.png')
    plt.clf()

    plt.xlim((0, 5))
    y2 = [math.log10(t) for t in parallel_res_10_i]
    cons_y = [math.log10(consistent_res)] * len(x)
    plt.plot(x, y2)
    plt.plot(x, cons_y, color='green')
    plt.xlabel('10^x number of threads')
    plt.ylabel('10^y total time (microseconds) of processing')
    plt.savefig('parl_res_10_i.png')


def main(config_filename, consistent_filename, parallel_filename):
    # Clearing files for output
    consistent_out = '{}.txt'.format(consistent_filename)
    consistent_cerr_out = '{}-cerr.txt'.format(consistent_filename)
    output_files = [consistent_out, consistent_cerr_out]
    parallel_out = parallel_filename + '{}.txt'
    parallel_cerr_out = parallel_filename + '{}-cerr.txt'
    for i in range(1, 5):
        output_files.append(parallel_out.format(i))
        output_files.append(parallel_cerr_out.format(i))
        output_files.append(parallel_out.format(i))
        output_files.append(parallel_cerr_out.format(10**i))
    clear_output(output_files)

    # Getting number of tests from arguments if this eligible
    if len(sys.argv) > 1:
        try:
            num_of_tests = int(sys.argv[1])
        except (TypeError, ValueError):
            print('Argument should be of type INT -- number of tests for each condition.')
            return -1
    else:
        num_of_tests = 10000
    print('Number of tests: {}'.format(num_of_tests))

    # Getting needed configurations from config file
    abs_err, rel_err = 0, 0
    try:
        abs_err, rel_err = parse_config(config_filename)
    except TypeError:
        pass

    # Looking for the path of consistent program
    path_to_consistent = find_path_to_file(consistent_filename)
    # config_for_consistent = '{}{}'.format('../' * (len(path_to_consistent.split('/')) - 1), config_filename)
    config_for_consistent = config_filename

    # Running consistent program
    for i in range(num_of_tests):
        run_program(consistent_filename, path_to_consistent, [config_for_consistent],
                    consistent_out, consistent_cerr_out)

    # Looking for the path of parallel program
    path_to_parallel = find_path_to_file(parallel_filename)
    #config_for_parallel = '{}{}'.format('../' * (len(path_to_parallel.split('/')) - 1), parallel_filename)
    config_for_parallel = config_filename

    # Running parallel program with different number of threads
    for i in range(1, 5):
        # for i
        change_num_of_threads_in_config(config_filename, i)
        for j in range(num_of_tests):
            run_program(parallel_filename, path_to_parallel, [config_for_parallel],
                        parallel_out.format(i), parallel_out.format(i))

        # for 10**i
        change_num_of_threads_in_config(config_filename, 10**i)
        for j in range(num_of_tests):
            run_program(parallel_filename, path_to_parallel, [config_for_parallel],
                        parallel_out.format(10**i), parallel_out.format(10**i))

    # Parse and checking consistent
    consistent_result = parse_and_check_out(consistent_out, abs_err, rel_err)

    # Parse and checking parallel
    parallel_result_i = []
    parallel_result_10_i = []
    for i in range(1, 5):
        parallel_result_i.append(parse_and_check_out(parallel_out.format(i), abs_err, rel_err))
        parallel_result_10_i.append(parse_and_check_out(parallel_out.format(10**i), abs_err, rel_err))

    # Print results
    print('Consistent: {} microseconds\n\n{}\n\n{}'.format(consistent_result,
          '\n'.join(['Parallel for {} threads: {} microseconds'.format(i + 1, el) for i, el in enumerate(parallel_result_i)]),
          '\n'.join(['Parallel for {} threads: {} microseconds'.format(10**(i + 1), el) for i, el in enumerate(parallel_result_10_i)])))

    # Plot results (uncomment the row below, if you need this)
    plot_results(consistent_result, parallel_result_i, parallel_result_10_i)


if __name__ == "__main__":
    config_filename = 'config.txt'
    consistent_filename = 'consistent'
    parallel_filename = 'parallel'
    main(config_filename, consistent_filename, parallel_filename)
