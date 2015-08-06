function initialize()
   io.write( "Fire Example" )
   io.write( " Machen " .. config.VERSION_MAJOR .. "." .. config.VERSION_MINOR .. "\n" )

   config.setLogLevel( "debug" )

   data.createLayer( "fire",
                     {
                        x0 = 0.0,
                        x1 = 20.0,
                        y0 = 0.0,
                        y1 = 20.0,
                        w = 200,
                        h = 200,
                        default = 0,
                     } )

   data.setLayerUpdate( "fire", "fireupdate" )

   config.addAgent( "fire", 20 )

   config.setvars( {
                      starttime = 0.0,
                      endtime = 10.0,
                      iters = 500,
                   } )
end

function startIteration( num )
   io.write( "Start iteration " .. num .. "\n" )
end

function endIteration( num )
   io.write( "End iteration " .. num .. "\n" )
   if num < 10 then
      raster.fire:save( "fire000"..num..".png" )
   elseif num < 100 then
      raster.fire:save( "fire00"..num..".png" )
   elseif num < 1000 then
      raster.fire:save( "fire0"..num..".png" )
   else
      raster.fire:save( "fire"..num..".png" )
   end
end

function endSimulation()
   io.write( "End Simulation\n" )
end

