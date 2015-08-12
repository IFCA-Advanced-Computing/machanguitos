function initialize()
   io.write( "Area Example\n" )
   io.write( " Machen " .. config.VERSION_MAJOR .. "." .. config.VERSION_MINOR .. "\n" )

   config.setLogLevel( "info" )

   data.loadLayer( "area", "area1.png",
                   {
                      x0 = 0.0,
                      x1 = 20.0,
                      y0 = 0.0,
                      y1 = 20.0,
                   } )

   data.setLayerUpdate( "area", "areaupdate" )

   config.addAgent( "dog", 10 )

   config.setvars( {
                      dbhost = "localhost",
                      dbport = 27017,
                      starttime = 0.0,
                      endtime = 10.0,
                      iters = 10,
                   } )
end

function startIteration( num )
   io.write( "Start iteration " .. num .. "\n" )
end

function endIteration( num )
   io.write( "End iteration " .. num .. "\n" )
end

function endSimulation()
   io.write( "End Simulation\n" )
end
