#!/bin/python
import csv
import random
import sys

MAX_VALUE = 2**16 - 1
OPERATIONS = ['PUT', 'GET', 'SCAN', 'DELETE']


def get_op(dims, key_max):
    op_code = random.choice(OPERATIONS)
    args = [random.choice(range(key_max))] # First arg is always a key
    if (op_code == 'PUT'):
        args += random.sample(range(MAX_VALUE), dims)
    elif (op_code == 'SCAN'):
        args += [random.choice(range(key_max))]
        args.sort()

    return [op_code] + args


def main(args):
    if ('-h' in args) or not(len(args) == 5):
        print('USAGE:\n\t%s <num_ops> <dimensions> <key_max> <folder>' % (args[0]))
        sys.exit(0)

    num_ops = int(args[1])
    dims = int(args[2])
    key_max = int(args[3])
    folder = args[4]
    fname = '%s/test_%s_%s_%s.wl' % (folder, num_ops, dims, key_max)

    with open(fname, 'w') as fid:
        writer = csv.writer(fid, delimiter=',')
        writer.writerow([num_ops])
        for _ in range(num_ops):
            writer.writerow(get_op(dims, key_max))


if __name__ == '__main__':
    main(sys.argv)
