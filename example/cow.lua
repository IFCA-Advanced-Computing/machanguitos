function Agent:init()
   io.write( "Agent init\n" )
   self.x = 4;
   if self.x then
      io.write( "self.x = " .. self.x .. "\n" )
   end
   self:test()
end

function Agent:update( delta )
   io.write( "Agent update = " .. delta .. "\n" )
end

function Agent:test()
   io.write( "testing Agent:test\n" )
end
