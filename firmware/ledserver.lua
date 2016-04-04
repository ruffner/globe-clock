s=net.createServer(net.TCP, 20)

saved = true

s:listen(1234, function(c)
   c:on("receive", function(c, l)
      if( string.sub(l, 1, 4) == "data") then
        if(saved) then saved=not saved end
        
        if (not saved) then
            print("saving new globe image")
            file.open("image", "w") 
            file.write(string.sub(l,5,-1))
        end
      elseif (l and not saved) then
            print("adding to file")
            file.write(l)
      end
      
   end) 
   c:on("disconnection", function(c)
      print("connection complete")
      
      total = 0 
      if file.open("image", "r") then
        repeat
            local line=file.read(210)

            if line then
                apa102.write(0xff,line)
                total = total + string.len(line)
            end           
        until not line 
            
        file.close() 
      end
      
   end)
end)


