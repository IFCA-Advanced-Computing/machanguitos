function Agent:init()
   io.write( ">> Agent init\n" )
   self.x = math.random( 10 );
   self.y = math.random( 10 );
   self.n = "hola"
   self:test()
end

function Agent:update( delta )
   io.write( ">> Agent update = " .. delta .. "\n" )
   self.x = self.x + 0.1*delta;
   self.y = self.y + 0.1*delta;
   io.write( "pos = (" .. self.x .. "," .. self.y .. ")\n" )
end

function Agent:test()
   io.write( "testing Agent:test\n" )
end
