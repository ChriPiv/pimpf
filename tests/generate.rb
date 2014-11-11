file = File.open("Clearer2.cpu", "rb")
contents = file.read

contents += " 00000000"

(4..255).each do |n|

    command = 0x22000000 + n*0x00010000

    contents += " "+command.to_s+" 00000000"

end

contents += " "+0x22010000.to_s+" "+0x30000000.to_s
File.open("final.cpu", 'w') { |file| file.write(contents) }
