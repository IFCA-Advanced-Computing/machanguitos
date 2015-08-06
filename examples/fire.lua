function Agent:init()
   io.write( ">> Fire init\n" )
   self.x = 1 + math.random( 18 )
   self.y = 16 + math.random( 10 ) / 10.0
   self.dx = 0.2 - math.random( 40 ) / 100.0
end

function Agent:update( delta )

   self.x = self.x + self.dx

   if self.x > 19.5 then
      self.x = 19.5
      self.dx = -self.dx
   end

   if self.x < 0 then
      self.x = 0
      self.dx = -self.dx
   end
   
   raster.fire:set( 0, self.x, self.y, 255 )

   io.write( ">> Fire update (" .. self.x .. "," .. self.y .. ")\n" )
end
