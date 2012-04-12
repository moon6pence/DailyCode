# 7L7W Ruby Day 1

number = rand(10)
answer = -1

while number != answer
	print "? "
	line = gets()
	
	answer = line.to_i()
	if answer == number
		puts "Exatly!"
	elsif answer > number
		puts "Too high"
	elsif answer < number
		puts "Too low"
	end
end
