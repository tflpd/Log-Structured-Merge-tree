#!/bin/python
import csv
import random
import sys

MAX_VALUE = 2**16 - 1


def main(args):
    if ('-h' in args) or not(len(args) == 4):
        print('USAGE:\n\t%s <rows> <dimensions> <folder>' % (args[0]))
        sys.exit(0)

    num_rows = int(args[1])
    dims = int(args[2])
    folder = args[3]
    fname = '%s/test_%s_%s.data' % (folder, num_rows, dims)

    possible_keys = random.sample(range(2 * num_rows), num_rows)

    with open(fname, 'w') as fid:
        writer = csv.writer(fid, delimiter=',')
        writer.writerow([num_rows, dims])
        for row_id in range(num_rows):
            tmp = [possible_keys[row_id]]
            for _ in range(dims):
                tmp.append(random.choice(range(MAX_VALUE)))
            writer.writerow(tmp)


if __name__ == '__main__':
    main(sys.argv)
