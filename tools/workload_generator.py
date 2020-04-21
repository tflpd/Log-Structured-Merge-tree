import csv
import random

NUM_OPS = 40
KEYS_RANGE = 10
VALUES_RANGE = 20
SCAN_RANGE = 30
OPS = ["PUT", "GET", "DELETE", "SCAN"]
OPS_WEIGHTS = [1, 1, 1, 1]
database = {}

# As long as the seed is set it will deterministically create the same results - useful for debugging
random.seed(1)

with open('instructions.csv', 'w', newline='') as instructions_file:
	with open('results.csv', 'w', newline='') as results_file:

		instr_writer = csv.writer(instructions_file)
		results_writer = csv.writer(results_file)

		instr_writer.writerow([NUM_OPS])
		results_writer.writerow([NUM_OPS])

		for op in range(NUM_OPS):

			operation = random.choices(OPS, OPS_WEIGHTS)

			if operation[0] == "PUT":
				key = random.randrange(0, KEYS_RANGE + 1)
				value1 = random.randrange(0, VALUES_RANGE + 1)
				value2 = random.randrange(0, VALUES_RANGE + 1)
				instr_writer.writerow(["PUT", key, value1, value2])
				results_writer.writerow(["SUCCESS"])
				database[key] = [value1, value2]

			elif operation[0] == "GET":
				key = random.randrange(0, KEYS_RANGE + 1)
				instr_writer.writerow(["GET",key])
				if key in database:
					v1 = database[key][0]
					v2 = database[key][1]
					results_writer.writerow(["SUCCESS", v1, v2])
				else:
					results_writer.writerow(["FAIL"])

			elif operation[0] == "DELETE":
				key = random.randrange(0, KEYS_RANGE + 1)
				instr_writer.writerow(["DELETE",key])
				results_writer.writerow(["SUCCESS"])
				database.pop(key, None)

			elif operation[0] == "SCAN":
				key1 = random.randrange(0, KEYS_RANGE + 1)
				key2 = random.randrange(0, KEYS_RANGE + 1)
				while key2 < key1:
					key2 = random.randrange(0, KEYS_RANGE + 1)
				instr_writer.writerow(["SCAN",key1,key2])
				range_scan_results = []
				for key, value in database.items():
					if key1 <= key <= key2:
						range_scan_results.append(key)
				if range_scan_results:
					range_scan_results.sort()
					if len(range_scan_results) > 30:
						range_scan_results = range_scan_results[:30]
					csv_results = ["SUCCESS"]
					for result in range_scan_results:
						csv_results.append(result)
						v1 = database[result][0]
						v2 = database[result][1]
						csv_results.append(v1)
						csv_results.append(v2)
					results_writer.writerow(csv_results)
				else:
					results_writer.writerow(["FAIL"])

			else:
				print("Invalid operation, you fucked up somehow")

#print(database)