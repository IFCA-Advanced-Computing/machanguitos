function initialize()
   io.write( "Cow Example\n" );
   io.write( " Machen " .. config.VERSION_MAJOR .. "." .. config.VERSION_MINOR .. "\n" );

   config.setLogLevel( "info" );

   data.createLayer( "grass",
                     {
                        x0 = 0.0,
                        x1 = 20.0,
                        y0 = 0.0,
                        y1 = 20.0,
                        w = 200,
                        h = 200,
                        default = 250,
                     } );

   data.loadLayer( "area", "area.png",
                   {
                      x0 = 0.0,
                      x1 = 20.0,
                      y0 = 0.0,
                      y1 = 20.0,
                   } );

   config.addAgent( "cow", 10 );
   config.addAgent( "cat", 3 );

   config.setvars( {
                      dbhost = "localhost",
                      dbport = 27017,
                      starttime = 0.0,
                      endtime = 10.0,
                      iters = 10,
                   } );
end

function startIteration( num )
   io.write( "Start iteration " .. num .. "\n" )
end

function endIteration( num )
   io.write( "End iteration " .. num .. "\n" )
   raster.grass:save( "output"..num..".png" )
end

function endSimulation()
   io.write( "End Simulation\n" )
end
