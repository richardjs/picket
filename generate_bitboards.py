print('const bitboard MOVES[][] = {{')
for row in range(7):
	right = ('0' * 8 * (row + 1))
	left = '0' * 8 * (6 - row)
	left = '0b' + left
	left = '\t' + left
	print(left + '11000000' + right)
	for col in range(6):
		print(left + ('0'*col) + '111' + ('0'*(5-col)) + right)
	print(left + '00000011' + right)
print('}, {')
print('}}')
