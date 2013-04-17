AgentClass.outVariables( 'x', 'y' )

function Agent:init()
   io.write( ">> Agent init\n" )
   self.x = math.random( 10 );
   self.y = math.random( 10 );
   self:test()
end

function Agent:update( delta )
   self.x = self.x + 0.1*delta;
   self.y = self.y + 0.1*delta;
   local rst = raster.grass;
   local grass = rst:get( self.x, self.y );
   rst:set( self.x, self.y, grass - 1 );
   io.write( " cow state = (" .. self.x .. "," .. self.y .. ") " .. grass .. "\n" );
end

function Agent:test()
   io.write( "testing Agent:test\n" )
end
