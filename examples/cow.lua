AgentClass.outVariables( 'x', 'y' )

function Agent:init()
   io.write( ">> Agent init\n" )
   self.x = 5 + math.random( 10 );
   self.y = 5 + math.random( 10 );
   self.dx = 0.2 - math.random( 20 ) / 100.0;
   self.dy = 0.2 - math.random( 40 ) / 100.0;
   self:test()
end

function Agent:update( delta )
   self.x = self.x + self.dx*delta;
   self.y = self.y + self.dy*delta;
   if self.x > 19.5 then
      self.x = 19.5
      self.dx = - self.dx
   end
   if self.x < 0.5 then
      self.x = 0.5
      self.dx = - self.dx
   end
   if self.y > 19.5 then
      self.y = 19.5
      self.dy = - self.dy
   end
   if self.y < 0.5 then
      self.y = 0.5
      self.dy = - self.dy
   end

   local rst = raster.grass;
   local grass = rst:get( 0, self.x, self.y );
   rst:set( 0, self.x, self.y, grass - 1 );
   io.write( " cow state = (" .. self.x .. "," .. self.y .. ") " .. grass .. "\n" );
end

function Agent:test()
   io.write( "testing Agent:test\n" )
end
