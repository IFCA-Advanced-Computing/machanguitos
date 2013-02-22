function Agent:init()
   io.write( ">> Agent init\n" )
   self.x = math.random( 10 );
   io.write( "self.x = " .. self.x .. "\n" )
   self:test()
end

function Agent:update( delta )
   io.write( ">> Agent update = " .. delta .. "\n" )
end

function Agent:test()
   io.write( "testing Agent:test\n" )
end
