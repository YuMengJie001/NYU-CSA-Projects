import subprocess


with open('trace.txt', 'r') as f:
    data = [int(line[-2:-1]) for line in f.readlines()]

for m in range(1, 21):
    with open('config.txt', 'w') as f:
        f.write(str(m) + '\n2')
    try:
        subprocess.check_call('./branchsimulator.out', shell=True)
    except subprocess.CalledProcessError:
        raise
    with open('trace.txt.out', 'r') as f:
        data_hat = [int(line[0]) for line in f.readlines()]

    print(1 - sum([data[i] == data_hat[i] for i in range(len(data))]) / len(data))