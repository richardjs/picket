print('const bitboard MOVES[2][64] = {{')
for row in range(7):
	right = ('0' * 8 * (row + 1)) + ','
	left = '0' * 8 * (6 - row)
	left = '0b' + left
	left = '\t' + left
	print(left + '00000011' + right)
	for col in range(6):
		print(left + ('0'*(5-col)) + '111' + ('0'*col) + right)
	print(left + '11000000' + right)

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
	print(left + '00000011' + right)
	for col in range(6):
		print(left + ('0'*(5-col)) + '111' + ('0'*(col)) + right)
	if row == 6:
		print(left + '11000000' + right[:-1])
	else:
		print(left + '11000000' + right)
print('}};')
