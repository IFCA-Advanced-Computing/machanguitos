function Agent:init()
   io.write( "** cat init\n" )
   self.val = false;
   self.cname = "micho" .. math.random(100)
end

function Agent:update( delta )
   if self.val then
      io.write( "** cat " .. self.cname .. " = " .. delta .. " TT \n" )
   else
      io.write( "** cat " .. self.cname .. " = " .. delta .. " FF \n" )
   end
   self.cname = "micho" .. math.random(100)
   self.val = not self.val
end
