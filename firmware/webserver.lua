srv=net.createServer(net.TCP)
srv:listen(80,function(conn)
conn:on("receive", function(client,payload)
tgtfile = string.sub(payload,string.find(payload,"GET /") +5,string.find(payload,"HTTP/") -2 )
if tgtfile == "" then tgtfile = "index.htm" end
local f = file.open(tgtfile,"r")
if f ~= nil then
client:send(file.read())
file.close()
else
client:send("<html>")
client:send(tgtfile.." not Found - 404 error.")
client:send("<a href='index.htm'>Home</a>")
end
client:close()
collectgarbage()
f = nil
tgtfile = nil
end)
end)