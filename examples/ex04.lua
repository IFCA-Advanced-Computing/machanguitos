function initialize()
   io.write( "Fire Example" )
   io.write( " Machen " .. config.VERSION_MAJOR .. "." .. config.VERSION_MINOR .. "\n" )

   config.setLogLevel( "warning" )

   data.createLayer( "daily",
                     {
                        x0 = 0.0,
                        x1 = 20.0,
                        y0 = 0.0,
                        y1 = 20.0,
                        w = 200,
                        h = 200,
                        default = 0,
                     } )

   data.setLayerUpdate( "daily", "dayupdate" )

   config.setvars( {
                      starttime = 0.0,
                      endtime = 24*100,
                      iters = 12*100,
                      randomseed = 1,
                      -- randomseed = os.clock(),
                   } )

end


function startIteration( num )
   -- empty
end

function endIteration( num )
   -- empty
end

function endSimulation()
   -- empty
end
