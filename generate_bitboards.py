print('#ifndef BITBOARDS_H')
print('#define BITBOARDS_H')
print('')
print('#include <stdint.h>')
print('')
print('typedef uint64_t bitboard;')
print('')
print('const bitboard STRAIGHT_MOVES[2][64] = {{')
for row in range(7):
	right = ('0' * 8 * (row + 1)) + ','
	left = '0' * 8 * (6 - row)
	left = '0b' + left
	left = '\t' + left
	print(left + '00000001' + right)
	for col in range(6):
		print(left + ('0'*(5-col)) + '010' + ('0'*col) + right)
	print(left + '10000000' + right)

for col in range(8):
	if col == 7:
		print('\t0b' + '0' * 64)
	else:
		print('\t0b' + '0' * 64 + ',')

print('}, {')

for col in range(8):
	print('\t0b' + '0' * 64 + ',')

for row in range(7):
	right = ('0' * 8 * (row)) + ','
	left = '0' * 8 * (7 - row)
	left = '0b' + left
	left = '\t' + left
	print(left + '00000001' + right)
	for col in range(6):
		print(left + ('0'*(5-col)) + '010' + ('0'*(col)) + right)
	if row == 6:
		print(left + '10000000' + right[:-1])
	else:
		print(left + '10000000' + right)
print('}};')

print('')

print('const bitboard DIAGONAL_MOVES[2][64] = {{')
for row in range(7):
	right = ('0' * 8 * (row + 1)) + ','
	left = '0' * 8 * (6 - row)
	left = '0b' + left
	left = '\t' + left
	print(left + '00000010' + right)
	for col in range(6):
		print(left + ('0'*(5-col)) + '101' + ('0'*col) + right)
	print(left + '01000000' + right)

for col in range(8):
	if col == 7:
		print('\t0b' + '0' * 64)
	else:
		print('\t0b' + '0' * 64 + ',')

print('}, {')

for col in range(8):
	print('\t0b' + '0' * 64 + ',')

for row in range(7):
	right = ('0' * 8 * (row)) + ','
	left = '0' * 8 * (7 - row)
	left = '0b' + left
	left = '\t' + left
	print(left + '00000010' + right)
	for col in range(6):
		print(left + ('0'*(5-col)) + '101' + ('0'*(col)) + right)
	if row == 6:
		print(left + '01000000' + right[:-1])
	else:
		print(left + '01000000' + right)
print('}};')
print('')
print('#endif')
