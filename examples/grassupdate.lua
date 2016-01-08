function Raster:update( delta, time )
   io.write( "RASTER update " .. delta .. " " .. time .. "\n" )

   for j = 0, self.height - 1 do
      for i = 0, self.width - 1 do
         newval = self:getpixel( 0, i, j ) - 30

         if newval < 0 then
            newval = 0
         end

         self:setpixel( 0, i, j, newval )
      end
   end
end
