function Raster:update( delta )
   io.write( "Fire update " .. delta .. "\n" )

   for j = 0, self.height - 2 do
      for i = 0, self.width - 1 do
         local p0 = 0
         if i > 0 then
            p0 = self:getpixel( 0, i-1, j+1 )
         end
         local p1 = 0
         if i < self.width - 1 then
            p1 = self:getpixel( 0, i+1, j+1 )
         end

         local p2 = self:getpixel( 0, i, j+1 ) 

         local p3 = self:getpixel( 0, i, j ) 

         local newval = (p0 + p1 + p2 + p3) / 4.0

         self:setpixel( 0, i, j, newval )
      end
   end

end
