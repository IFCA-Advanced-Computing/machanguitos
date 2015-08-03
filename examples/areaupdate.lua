function Raster:update( delta )
   io.write( "AREA update " .. delta .. "\n" )

   self.loadflag = not self.loadflag
   if self.loadflag then
      self:load( "area2.png" )
   else
      self:load( "area1.png" )
   end

   io.write( " new size " .. self.width .. "\n" )
end
