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
print('const bitboard WIN_SPACES[2] = {')
print('\t0b' + '11111111' + '0'*56 + ',')
print('\t0b' + '0'*56 + '11111111')
print('};')
print('')

print('const bitboard MOVE_CONES[2][64] = {{')

for i in range(64):
	left = i
	right = i
	bitboard = 0
	while left < 64:
		for bit in range(left, right+1):
			bitboard |= (1 << bit)
		if left % 8 == 0:
			left += 8
		else:
			left += 7
		if (right + 1) % 8 == 0:
			right += 8
		else:
			right += 9
	bitboard = bin(bitboard)[2:]
	if i != 63:
		print('\t', '0b' + '0'*(64-len(bitboard)) + bitboard + ',')
	else:
		print('\t', '0b' + '0'*(64-len(bitboard)) + bitboard)

print('}, {')

for i in range(64):
	left = i
	right = i
	bitboard = 0
	while left >= 0:
		for bit in range(left, right+1):
			bitboard |= (1 << bit)
		if left % 8 == 0:
			left -= 8
		else:
			left -= 9
		if (right + 1) % 8 == 0:
			right -= 8
		else:
			right -= 7
	bitboard = bin(bitboard)[2:]
	if i != 63:
		print('\t', '0b' + '0'*(64-len(bitboard)) + bitboard + ',')
	else:
		print('\t', '0b' + '0'*(64-len(bitboard)) + bitboard)

print('}};')
print('')
print('#endif')
