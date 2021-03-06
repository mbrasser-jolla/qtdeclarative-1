import QtQuick 2.0
import QtTest 1.0
import "testhelper.js" as Helper
Canvas {
   id:canvas; width:100;height:50; renderTarget: Canvas.Image
   Component.onCompleted: {
       canvas.loadImage('green.png');
       canvas.loadImage('red.png');
       canvas.loadImage('rgrg-256x256.png');
       canvas.loadImage('ggrr-256x256.png');
       canvas.loadImage('broken.png');
   }

   TestCase {
       //TODO
       name: "image"; when: windowShown
       function test_3args() {
           //make sure all images are loaded
           wait(200);
           var ctx = canvas.getContext('2d');
           ctx.reset();
           ctx.drawImage('green.png', 0, 0);
           ctx.drawImage('red.png', -100, 0);
           ctx.drawImage('red.png', 100, 0);
           ctx.drawImage('red.png', 0, -50);
           ctx.drawImage('red.png', 0, 50);

           verify(Helper.comparePixel(ctx, 0,0, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 99,0, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 0,49, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 99,49, 0,255,0,255,2));

      }
       function test_5args() {
           var ctx = canvas.getContext('2d');
           ctx.reset();
           ctx.fillStyle = '#f00';
           ctx.fillRect(0, 0, 100, 50);
           ctx.drawImage('green.png', 50, 0, 50, 50);
           ctx.drawImage('red.png', 0, 0, 50, 50);
           ctx.fillStyle = '#0f0';
           ctx.fillRect(0, 0, 50, 50);

           verify(Helper.comparePixel(ctx, 0,0, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 99,0, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 0,49, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 99,49, 0,255,0,255,2));

      }
       function test_9args() {
           var ctx = canvas.getContext('2d');
           ctx.reset();
           ctx.fillStyle = '#f00';
           ctx.fillRect(0, 0, 100, 50);
           ctx.drawImage('green.png', 0, 0, 100, 50, 0, 0, 100, 50);
           verify(Helper.comparePixel(ctx, 0,0, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 99,0, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 0,49, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 99,49, 0,255,0,255,2));

           ctx.reset();

           ctx.fillStyle = '#f00';
           ctx.fillRect(0, 0, 100, 50);
           ctx.drawImage('green.png', 0, 0, 100, 50, 0, 0, 100, 50);
           ctx.drawImage('red.png', 0, 0, 100, 50, -100, 0, 100, 50);
           ctx.drawImage('red.png', 0, 0, 100, 50, 100, 0, 100, 50);
           ctx.drawImage('red.png', 0, 0, 100, 50, 0, -50, 100, 50);
           ctx.drawImage('red.png', 0, 0, 100, 50, 0, 50, 100, 50);
           verify(Helper.comparePixel(ctx, 0,0, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 99,0, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 0,49, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 99,49, 0,255,0,255,2));


           ctx.fillStyle = '#f00';
           ctx.fillRect(0, 0, 100, 50);
           ctx.drawImage('green.png', 1, 1, 1, 1, 0, 0, 100, 50);
           ctx.drawImage('red.png', 0, 0, 100, 50, -50, 0, 50, 50);
           ctx.drawImage('red.png', 0, 0, 100, 50, 100, 0, 50, 50);
           ctx.drawImage('red.png', 0, 0, 100, 50, 0, -25, 100, 25);
           ctx.drawImage('red.png', 0, 0, 100, 50, 0, 50, 100, 25);
           verify(Helper.comparePixel(ctx, 0,0, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 99,0, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 0,49, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 99,49, 0,255,0,255,2));

           ctx.fillStyle = '#f00';
           ctx.fillRect(0, 0, 100, 50);
           ctx.drawImage('rgrg-256x256.png', 140, 20, 100, 50, 0, 0, 100, 50);
           verify(Helper.comparePixel(ctx, 0,0, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 99,0, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 0,49, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 99,49, 0,255,0,255,2));

           ctx.fillStyle = '#f00';
           ctx.fillRect(0, 0, 100, 50);
           ctx.drawImage('rgrg-256x256.png', 0, 0, 256, 256, 0, 0, 100, 50);
           ctx.fillStyle = '#0f0';
           ctx.fillRect(0, 0, 51, 26);
           ctx.fillRect(49, 24, 51, 26);
           verify(Helper.comparePixel(ctx, 0,0, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 99,0, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 0,49, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 99,49, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 20,20, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 80,20, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 20,30, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 80,30, 0,255,0,255,2));

       }
       function test_animated() {
           var ctx = canvas.getContext('2d');
           ctx.reset();
           //should animated image be supported at all?
      }
       function test_clip() {
           var ctx = canvas.getContext('2d');
           ctx.reset();
           ctx.fillStyle = '#0f0';
           ctx.fillRect(0, 0, 100, 50);
           ctx.rect(-10, -10, 1, 1);
           ctx.clip();
           ctx.drawImage('red.png', 0, 0);
           verify(Helper.comparePixel(ctx, 50,25, 0,255,0,255,2));


      }
       function test_self() {
           var ctx = canvas.getContext('2d');
           ctx.reset();
           ignoreWarning("QImage::scaled: Image is a null image");
           ignoreWarning("QImage::scaled: Image is a null image");

           ctx.fillStyle = '#0f0';
           ctx.fillRect(0, 0, 50, 50);
           ctx.fillStyle = '#f00';
           ctx.fillRect(50, 0, 50, 50);
           ctx.drawImage(canvas, 50, 0);

           verify(Helper.comparePixel(ctx, 0,0, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 99,0, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 0,49, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 99,49, 0,255,0,255,2));

           ctx.reset();
           ctx.fillStyle = '#0f0';
           ctx.fillRect(0, 1, 100, 49);
           ctx.fillStyle = '#f00';
           ctx.fillRect(0, 0, 100, 1);
           ctx.drawImage(canvas, 0, 1);
           ctx.fillStyle = '#0f0';
           ctx.fillRect(0, 0, 100, 2);

           verify(Helper.comparePixel(ctx, 0,0, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 99,0, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 0,49, 0,255,0,255,2));
           verify(Helper.comparePixel(ctx, 99,49, 0,255,0,255,2));


       }

       function test_outsidesource() {
           var ctx = canvas.getContext('2d');
           ctx.reset();

           ctx.drawImage('green.png', 10.5, 10.5, 89.5, 39.5, 0, 0, 100, 50);
           ctx.drawImage('green.png', 5.5, 5.5, -5.5, -5.5, 0, 0, 100, 50);
           ctx.drawImage('green.png', 100, 50, -5, -5, 0, 0, 100, 50);
           try { var err = false;
             ctx.drawImage('red.png', -0.001, 0, 100, 50, 0, 0, 100, 50);
           } catch (e) { if (e.code != DOMException.INDEX_SIZE_ERR) fail("Failed assertion: expected exception of type INDEX_SIZE_ERR, got: "+e.message); err = true; } finally { verify(err, "should throw exception of type INDEX_SIZE_ERR: ctx.drawImage('red.png', -0.001, 0, 100, 50, 0, 0, 100, 50)"); }
           try { var err = false;
             ctx.drawImage('red.png', 0, -0.001, 100, 50, 0, 0, 100, 50);
           } catch (e) { if (e.code != DOMException.INDEX_SIZE_ERR) fail("Failed assertion: expected exception of type INDEX_SIZE_ERR, got: "+e.message); err = true; } finally { verify(err, "should throw exception of type INDEX_SIZE_ERR: ctx.drawImage('red.png', 0, -0.001, 100, 50, 0, 0, 100, 50)"); }
           try { var err = false;
             ctx.drawImage('red.png', 0, 0, 100.001, 50, 0, 0, 100, 50);
           } catch (e) { if (e.code != DOMException.INDEX_SIZE_ERR) fail("Failed assertion: expected exception of type INDEX_SIZE_ERR, got: "+e.message); err = true; } finally { verify(err, "should throw exception of type INDEX_SIZE_ERR: ctx.drawImage('red.png', 0, 0, 100.001, 50, 0, 0, 100, 50)"); }
           try { var err = false;
             ctx.drawImage('red.png', 0, 0, 100, 50.001, 0, 0, 100, 50);
           } catch (e) { if (e.code != DOMException.INDEX_SIZE_ERR) fail("Failed assertion: expected exception of type INDEX_SIZE_ERR, got: "+e.message); err = true; } finally { verify(err, "should throw exception of type INDEX_SIZE_ERR: ctx.drawImage('red.png', 0, 0, 100, 50.001, 0, 0, 100, 50)"); }
           try { var err = false;
             ctx.drawImage('red.png', 50, 0, 50.001, 50, 0, 0, 100, 50);
           } catch (e) { if (e.code != DOMException.INDEX_SIZE_ERR) fail("Failed assertion: expected exception of type INDEX_SIZE_ERR, got: "+e.message); err = true; } finally { verify(err, "should throw exception of type INDEX_SIZE_ERR: ctx.drawImage('red.png', 50, 0, 50.001, 50, 0, 0, 100, 50)"); }
           try { var err = false;
             ctx.drawImage('red.png', 0, 0, -5, 5, 0, 0, 100, 50);
           } catch (e) { if (e.code != DOMException.INDEX_SIZE_ERR) fail("Failed assertion: expected exception of type INDEX_SIZE_ERR, got: "+e.message); err = true; } finally { verify(err, "should throw exception of type INDEX_SIZE_ERR: ctx.drawImage('red.png', 0, 0, -5, 5, 0, 0, 100, 50)"); }
           try { var err = false;
             ctx.drawImage('red.png', 0, 0, 5, -5, 0, 0, 100, 50);
           } catch (e) { if (e.code != DOMException.INDEX_SIZE_ERR) fail("Failed assertion: expected exception of type INDEX_SIZE_ERR, got: "+e.message); err = true; } finally { verify(err, "should throw exception of type INDEX_SIZE_ERR: ctx.drawImage('red.png', 0, 0, 5, -5, 0, 0, 100, 50)"); }
//           try { var err = false;
//             ctx.drawImage('red.png', 110, 60, -20, -20, 0, 0, 100, 50);
//           } catch (e) { if (e.code != DOMException.INDEX_SIZE_ERR) fail("Failed assertion: expected exception of type INDEX_SIZE_ERR, got: "+e.message); err = true; } finally { verify(err, "should throw exception of type INDEX_SIZE_ERR: ctx.drawImage('red.png', 110, 60, -20, -20, 0, 0, 100, 50)"); }
//           verify(Helper.comparePixel(ctx, 50,25, 0,255,0,255,2));

       }

       function test_null() {
           var ctx = canvas.getContext('2d');
           ctx.reset();

           try { var err = false;
             ctx.drawImage(null, 0, 0);
           } catch (e) { if (e.code != DOMException.TYPE_MISMATCH_ERR) fail("Failed assertion: expected exception of type TYPE_MISMATCH_ERR, got: "+e.message); err = true; } finally { verify(err, "should throw exception of type TYPE_MISMATCH_ERR: ctx.drawImage(null, 0, 0)"); }

       }

       function test_composite() {
           var ctx = canvas.getContext('2d');
           ctx.reset();
           ctx.fillStyle = '#0f0';
           ctx.fillRect(0, 0, 100, 50);
           ctx.globalCompositeOperation = 'destination-over';
           ctx.drawImage('red.png', 0, 0);
           verify(Helper.comparePixel(ctx, 50,25, 0,255,0,255,2));

      }
       function test_path() {
           var ctx = canvas.getContext('2d');
           ctx.reset();
      }
       function test_transform() {
           var ctx = canvas.getContext('2d');
           ctx.reset();
           ctx.fillStyle = '#0f0';
           ctx.fillRect(0, 0, 100, 50);
           ctx.translate(100, 0);
           ctx.drawImage('red.png', 0, 0);
           verify(Helper.comparePixel(ctx, 50,25, 0,255,0,255,2));

      }

       function test_imageitem() {
           var ctx = canvas.getContext('2d');
           ctx.reset();
           //TODO
       }

       function test_imageData() {
           var ctx = canvas.getContext('2d');
           ctx.reset();
           //TODO
       }

       function test_wrongtype() {
           var ctx = canvas.getContext('2d');
           ctx.reset();

           try { var err = false;
             ctx.drawImage(undefined, 0, 0);
           } catch (e) { if (e.code != DOMException.TYPE_MISMATCH_ERR) fail("Failed assertion: expected exception of type TYPE_MISMATCH_ERR, got: "+e.message); err = true; } finally { verify(err, "should throw exception of type TYPE_MISMATCH_ERR: ctx.drawImage(undefined, 0, 0)"); }
           try { var err = false;
             ctx.drawImage(0, 0, 0);
           } catch (e) { if (e.code != DOMException.TYPE_MISMATCH_ERR) fail("Failed assertion: expected exception of type TYPE_MISMATCH_ERR, got: "+e.message); err = true; } finally { verify(err, "should throw exception of type TYPE_MISMATCH_ERR: ctx.drawImage(0, 0, 0)"); }
           try { var err = false;
             ctx.drawImage("", 0, 0);
           } catch (e) { if (e.code != DOMException.TYPE_MISMATCH_ERR) fail("Failed assertion: expected exception of type TYPE_MISMATCH_ERR, got: "+e.message); err = true; } finally { verify(err, "should throw exception of type TYPE_MISMATCH_ERR: ctx.drawImage(\"\", 0, 0)"); }
       }

       function test_nonfinite() {
           var ctx = canvas.getContext('2d');
           ctx.reset();
           ignoreWarning("QImage::scaled: Image is a null image");
           ignoreWarning("QImage::scaled: Image is a null image");
           ignoreWarning("QImage::scaled: Image is a null image");
           ignoreWarning("QImage::scaled: Image is a null image");
           ctx.fillStyle = '#0f0';
           ctx.fillRect(0, 0, 100, 50);
           var red = 'red.png';
           ctx.drawImage(red, Infinity, 0);
           ctx.drawImage(red, -Infinity, 0);
           ctx.drawImage(red, NaN, 0);
           ctx.drawImage(red, 0, Infinity);
           ctx.drawImage(red, 0, -Infinity);
           ctx.drawImage(red, 0, NaN);
           ctx.drawImage(red, Infinity, Infinity);
           ctx.drawImage(red, Infinity, 0, 100, 50);
           ctx.drawImage(red, -Infinity, 0, 100, 50);
           ctx.drawImage(red, NaN, 0, 100, 50);
           ctx.drawImage(red, 0, Infinity, 100, 50);
           ctx.drawImage(red, 0, -Infinity, 100, 50);
           ctx.drawImage(red, 0, NaN, 100, 50);
           ctx.drawImage(red, 0, 0, Infinity, 50);
           ctx.drawImage(red, 0, 0, -Infinity, 50);
           ctx.drawImage(red, 0, 0, NaN, 50);
           ctx.drawImage(red, 0, 0, 100, Infinity);
           ctx.drawImage(red, 0, 0, 100, -Infinity);
           ctx.drawImage(red, 0, 0, 100, NaN);
           ctx.drawImage(red, Infinity, Infinity, 100, 50);
           ctx.drawImage(red, Infinity, Infinity, Infinity, 50);
           ctx.drawImage(red, Infinity, Infinity, Infinity, Infinity);
           ctx.drawImage(red, Infinity, Infinity, 100, Infinity);
           ctx.drawImage(red, Infinity, 0, Infinity, 50);
           ctx.drawImage(red, Infinity, 0, Infinity, Infinity);
           ctx.drawImage(red, Infinity, 0, 100, Infinity);
           ctx.drawImage(red, 0, Infinity, Infinity, 50);
           ctx.drawImage(red, 0, Infinity, Infinity, Infinity);
           ctx.drawImage(red, 0, Infinity, 100, Infinity);
           ctx.drawImage(red, 0, 0, Infinity, Infinity);
           ctx.drawImage(red, Infinity, 0, 100, 50, 0, 0, 100, 50);
           ctx.drawImage(red, -Infinity, 0, 100, 50, 0, 0, 100, 50);
           ctx.drawImage(red, NaN, 0, 100, 50, 0, 0, 100, 50);
           ctx.drawImage(red, 0, Infinity, 100, 50, 0, 0, 100, 50);
           ctx.drawImage(red, 0, -Infinity, 100, 50, 0, 0, 100, 50);
           ctx.drawImage(red, 0, NaN, 100, 50, 0, 0, 100, 50);
           ctx.drawImage(red, 0, 0, Infinity, 50, 0, 0, 100, 50);
           ctx.drawImage(red, 0, 0, -Infinity, 50, 0, 0, 100, 50);
           ctx.drawImage(red, 0, 0, NaN, 50, 0, 0, 100, 50);
           ctx.drawImage(red, 0, 0, 100, Infinity, 0, 0, 100, 50);
           ctx.drawImage(red, 0, 0, 100, -Infinity, 0, 0, 100, 50);
           ctx.drawImage(red, 0, 0, 100, NaN, 0, 0, 100, 50);
           ctx.drawImage(red, 0, 0, 100, 50, Infinity, 0, 100, 50);
           ctx.drawImage(red, 0, 0, 100, 50, -Infinity, 0, 100, 50);
           ctx.drawImage(red, 0, 0, 100, 50, NaN, 0, 100, 50);
           ctx.drawImage(red, 0, 0, 100, 50, 0, Infinity, 100, 50);
           ctx.drawImage(red, 0, 0, 100, 50, 0, -Infinity, 100, 50);
           ctx.drawImage(red, 0, 0, 100, 50, 0, NaN, 100, 50);
           ctx.drawImage(red, 0, 0, 100, 50, 0, 0, Infinity, 50);
           ctx.drawImage(red, 0, 0, 100, 50, 0, 0, -Infinity, 50);
           ctx.drawImage(red, 0, 0, 100, 50, 0, 0, NaN, 50);
           ctx.drawImage(red, 0, 0, 100, 50, 0, 0, 100, Infinity);
           ctx.drawImage(red, 0, 0, 100, 50, 0, 0, 100, -Infinity);
           ctx.drawImage(red, 0, 0, 100, 50, 0, 0, 100, NaN);
           ctx.drawImage(red, Infinity, Infinity, 100, 50, 0, 0, 100, 50);
           ctx.drawImage(red, Infinity, Infinity, Infinity, 50, 0, 0, 100, 50);
           ctx.drawImage(red, Infinity, Infinity, Infinity, Infinity, 0, 0, 100, 50);
           ctx.drawImage(red, Infinity, Infinity, Infinity, Infinity, Infinity, 0, 100, 50);
           ctx.drawImage(red, Infinity, Infinity, Infinity, Infinity, Infinity, Infinity, 100, 50);
           ctx.drawImage(red, Infinity, Infinity, Infinity, Infinity, Infinity, Infinity, Infinity, 50);
           ctx.drawImage(red, Infinity, Infinity, Infinity, Infinity, Infinity, Infinity, Infinity, Infinity);
           ctx.drawImage(red, Infinity, Infinity, Infinity, Infinity, Infinity, Infinity, 100, Infinity);
           ctx.drawImage(red, Infinity, Infinity, Infinity, Infinity, Infinity, 0, Infinity, 50);
           ctx.drawImage(red, Infinity, Infinity, Infinity, Infinity, Infinity, 0, Infinity, Infinity);
           ctx.drawImage(red, Infinity, Infinity, Infinity, Infinity, Infinity, 0, 100, Infinity);
           ctx.drawImage(red, Infinity, Infinity, Infinity, Infinity, 0, Infinity, 100, 50);
           ctx.drawImage(red, Infinity, Infinity, Infinity, Infinity, 0, Infinity, Infinity, 50);
           ctx.drawImage(red, Infinity, Infinity, Infinity, Infinity, 0, Infinity, Infinity, Infinity);
           ctx.drawImage(red, Infinity, Infinity, Infinity, Infinity, 0, Infinity, 100, Infinity);
           ctx.drawImage(red, Infinity, Infinity, Infinity, Infinity, 0, 0, Infinity, 50);
           ctx.drawImage(red, Infinity, Infinity, Infinity, Infinity, 0, 0, Infinity, Infinity);
           ctx.drawImage(red, Infinity, Infinity, Infinity, Infinity, 0, 0, 100, Infinity);
           ctx.drawImage(red, Infinity, Infinity, Infinity, 50, Infinity, 0, 100, 50);
           ctx.drawImage(red, Infinity, Infinity, Infinity, 50, Infinity, Infinity, 100, 50);
           ctx.drawImage(red, Infinity, Infinity, Infinity, 50, Infinity, Infinity, Infinity, 50);
           ctx.drawImage(red, Infinity, Infinity, Infinity, 50, Infinity, Infinity, Infinity, Infinity);
           ctx.drawImage(red, Infinity, Infinity, Infinity, 50, Infinity, Infinity, 100, Infinity);
           ctx.drawImage(red, Infinity, Infinity, Infinity, 50, Infinity, 0, Infinity, 50);
           ctx.drawImage(red, Infinity, Infinity, Infinity, 50, Infinity, 0, Infinity, Infinity);
           ctx.drawImage(red, Infinity, Infinity, Infinity, 50, Infinity, 0, 100, Infinity);
           ctx.drawImage(red, Infinity, Infinity, Infinity, 50, 0, Infinity, 100, 50);
           ctx.drawImage(red, Infinity, Infinity, Infinity, 50, 0, Infinity, Infinity, 50);
           ctx.drawImage(red, Infinity, Infinity, Infinity, 50, 0, Infinity, Infinity, Infinity);
           ctx.drawImage(red, Infinity, Infinity, Infinity, 50, 0, Infinity, 100, Infinity);
           ctx.drawImage(red, Infinity, Infinity, Infinity, 50, 0, 0, Infinity, 50);
           ctx.drawImage(red, Infinity, Infinity, Infinity, 50, 0, 0, Infinity, Infinity);
           ctx.drawImage(red, Infinity, Infinity, Infinity, 50, 0, 0, 100, Infinity);
           ctx.drawImage(red, Infinity, Infinity, 100, Infinity, 0, 0, 100, 50);
           ctx.drawImage(red, Infinity, Infinity, 100, Infinity, Infinity, 0, 100, 50);
           ctx.drawImage(red, Infinity, Infinity, 100, Infinity, Infinity, Infinity, 100, 50);
           ctx.drawImage(red, Infinity, Infinity, 100, Infinity, Infinity, Infinity, Infinity, 50);
           ctx.drawImage(red, Infinity, Infinity, 100, Infinity, Infinity, Infinity, Infinity, Infinity);
           ctx.drawImage(red, Infinity, Infinity, 100, Infinity, Infinity, Infinity, 100, Infinity);
           ctx.drawImage(red, Infinity, Infinity, 100, Infinity, Infinity, 0, Infinity, 50);
           ctx.drawImage(red, Infinity, Infinity, 100, Infinity, Infinity, 0, Infinity, Infinity);
           ctx.drawImage(red, Infinity, Infinity, 100, Infinity, Infinity, 0, 100, Infinity);
           ctx.drawImage(red, Infinity, Infinity, 100, Infinity, 0, Infinity, 100, 50);
           ctx.drawImage(red, Infinity, Infinity, 100, Infinity, 0, Infinity, Infinity, 50);
           ctx.drawImage(red, Infinity, Infinity, 100, Infinity, 0, Infinity, Infinity, Infinity);
           ctx.drawImage(red, Infinity, Infinity, 100, Infinity, 0, Infinity, 100, Infinity);
           ctx.drawImage(red, Infinity, Infinity, 100, Infinity, 0, 0, Infinity, 50);
           ctx.drawImage(red, Infinity, Infinity, 100, Infinity, 0, 0, Infinity, Infinity);
           ctx.drawImage(red, Infinity, Infinity, 100, Infinity, 0, 0, 100, Infinity);
           ctx.drawImage(red, Infinity, Infinity, 100, 50, Infinity, 0, 100, 50);
           ctx.drawImage(red, Infinity, Infinity, 100, 50, Infinity, Infinity, 100, 50);
           ctx.drawImage(red, Infinity, Infinity, 100, 50, Infinity, Infinity, Infinity, 50);
           ctx.drawImage(red, Infinity, Infinity, 100, 50, Infinity, Infinity, Infinity, Infinity);
           ctx.drawImage(red, Infinity, Infinity, 100, 50, Infinity, Infinity, 100, Infinity);
           ctx.drawImage(red, Infinity, Infinity, 100, 50, Infinity, 0, Infinity, 50);
           ctx.drawImage(red, Infinity, Infinity, 100, 50, Infinity, 0, Infinity, Infinity);
           ctx.drawImage(red, Infinity, Infinity, 100, 50, Infinity, 0, 100, Infinity);
           ctx.drawImage(red, Infinity, Infinity, 100, 50, 0, Infinity, 100, 50);
           ctx.drawImage(red, Infinity, Infinity, 100, 50, 0, Infinity, Infinity, 50);
           ctx.drawImage(red, Infinity, Infinity, 100, 50, 0, Infinity, Infinity, Infinity);
           ctx.drawImage(red, Infinity, Infinity, 100, 50, 0, Infinity, 100, Infinity);
           ctx.drawImage(red, Infinity, Infinity, 100, 50, 0, 0, Infinity, 50);
           ctx.drawImage(red, Infinity, Infinity, 100, 50, 0, 0, Infinity, Infinity);
           ctx.drawImage(red, Infinity, Infinity, 100, 50, 0, 0, 100, Infinity);
           ctx.drawImage(red, Infinity, 0, Infinity, 50, 0, 0, 100, 50);
           ctx.drawImage(red, Infinity, 0, Infinity, Infinity, 0, 0, 100, 50);
           ctx.drawImage(red, Infinity, 0, Infinity, Infinity, Infinity, 0, 100, 50);
           ctx.drawImage(red, Infinity, 0, Infinity, Infinity, Infinity, Infinity, 100, 50);
           ctx.drawImage(red, Infinity, 0, Infinity, Infinity, Infinity, Infinity, Infinity, 50);
           ctx.drawImage(red, Infinity, 0, Infinity, Infinity, Infinity, Infinity, Infinity, Infinity);
           ctx.drawImage(red, Infinity, 0, Infinity, Infinity, Infinity, Infinity, 100, Infinity);
           ctx.drawImage(red, Infinity, 0, Infinity, Infinity, Infinity, 0, Infinity, 50);
           ctx.drawImage(red, Infinity, 0, Infinity, Infinity, Infinity, 0, Infinity, Infinity);
           ctx.drawImage(red, Infinity, 0, Infinity, Infinity, Infinity, 0, 100, Infinity);
           ctx.drawImage(red, Infinity, 0, Infinity, Infinity, 0, Infinity, 100, 50);
           ctx.drawImage(red, Infinity, 0, Infinity, Infinity, 0, Infinity, Infinity, 50);
           ctx.drawImage(red, Infinity, 0, Infinity, Infinity, 0, Infinity, Infinity, Infinity);
           ctx.drawImage(red, Infinity, 0, Infinity, Infinity, 0, Infinity, 100, Infinity);
           ctx.drawImage(red, Infinity, 0, Infinity, Infinity, 0, 0, Infinity, 50);
           ctx.drawImage(red, Infinity, 0, Infinity, Infinity, 0, 0, Infinity, Infinity);
           ctx.drawImage(red, Infinity, 0, Infinity, Infinity, 0, 0, 100, Infinity);
           ctx.drawImage(red, Infinity, 0, Infinity, 50, Infinity, 0, 100, 50);
           ctx.drawImage(red, Infinity, 0, Infinity, 50, Infinity, Infinity, 100, 50);
           ctx.drawImage(red, Infinity, 0, Infinity, 50, Infinity, Infinity, Infinity, 50);
           ctx.drawImage(red, Infinity, 0, Infinity, 50, Infinity, Infinity, Infinity, Infinity);
           ctx.drawImage(red, Infinity, 0, Infinity, 50, Infinity, Infinity, 100, Infinity);
           ctx.drawImage(red, Infinity, 0, Infinity, 50, Infinity, 0, Infinity, 50);
           ctx.drawImage(red, Infinity, 0, Infinity, 50, Infinity, 0, Infinity, Infinity);
           ctx.drawImage(red, Infinity, 0, Infinity, 50, Infinity, 0, 100, Infinity);
           ctx.drawImage(red, Infinity, 0, Infinity, 50, 0, Infinity, 100, 50);
           ctx.drawImage(red, Infinity, 0, Infinity, 50, 0, Infinity, Infinity, 50);
           ctx.drawImage(red, Infinity, 0, Infinity, 50, 0, Infinity, Infinity, Infinity);
           ctx.drawImage(red, Infinity, 0, Infinity, 50, 0, Infinity, 100, Infinity);
           ctx.drawImage(red, Infinity, 0, Infinity, 50, 0, 0, Infinity, 50);
           ctx.drawImage(red, Infinity, 0, Infinity, 50, 0, 0, Infinity, Infinity);
           ctx.drawImage(red, Infinity, 0, Infinity, 50, 0, 0, 100, Infinity);
           ctx.drawImage(red, Infinity, 0, 100, Infinity, 0, 0, 100, 50);
           ctx.drawImage(red, Infinity, 0, 100, Infinity, Infinity, 0, 100, 50);
           ctx.drawImage(red, Infinity, 0, 100, Infinity, Infinity, Infinity, 100, 50);
           ctx.drawImage(red, Infinity, 0, 100, Infinity, Infinity, Infinity, Infinity, 50);
           ctx.drawImage(red, Infinity, 0, 100, Infinity, Infinity, Infinity, Infinity, Infinity);
           ctx.drawImage(red, Infinity, 0, 100, Infinity, Infinity, Infinity, 100, Infinity);
           ctx.drawImage(red, Infinity, 0, 100, Infinity, Infinity, 0, Infinity, 50);
           ctx.drawImage(red, Infinity, 0, 100, Infinity, Infinity, 0, Infinity, Infinity);
           ctx.drawImage(red, Infinity, 0, 100, Infinity, Infinity, 0, 100, Infinity);
           ctx.drawImage(red, Infinity, 0, 100, Infinity, 0, Infinity, 100, 50);
           ctx.drawImage(red, Infinity, 0, 100, Infinity, 0, Infinity, Infinity, 50);
           ctx.drawImage(red, Infinity, 0, 100, Infinity, 0, Infinity, Infinity, Infinity);
           ctx.drawImage(red, Infinity, 0, 100, Infinity, 0, Infinity, 100, Infinity);
           ctx.drawImage(red, Infinity, 0, 100, Infinity, 0, 0, Infinity, 50);
           ctx.drawImage(red, Infinity, 0, 100, Infinity, 0, 0, Infinity, Infinity);
           ctx.drawImage(red, Infinity, 0, 100, Infinity, 0, 0, 100, Infinity);
           ctx.drawImage(red, Infinity, 0, 100, 50, Infinity, 0, 100, 50);
           ctx.drawImage(red, Infinity, 0, 100, 50, Infinity, Infinity, 100, 50);
           ctx.drawImage(red, Infinity, 0, 100, 50, Infinity, Infinity, Infinity, 50);
           ctx.drawImage(red, Infinity, 0, 100, 50, Infinity, Infinity, Infinity, Infinity);
           ctx.drawImage(red, Infinity, 0, 100, 50, Infinity, Infinity, 100, Infinity);
           ctx.drawImage(red, Infinity, 0, 100, 50, Infinity, 0, Infinity, 50);
           ctx.drawImage(red, Infinity, 0, 100, 50, Infinity, 0, Infinity, Infinity);
           ctx.drawImage(red, Infinity, 0, 100, 50, Infinity, 0, 100, Infinity);
           ctx.drawImage(red, Infinity, 0, 100, 50, 0, Infinity, 100, 50);
           ctx.drawImage(red, Infinity, 0, 100, 50, 0, Infinity, Infinity, 50);
           ctx.drawImage(red, Infinity, 0, 100, 50, 0, Infinity, Infinity, Infinity);
           ctx.drawImage(red, Infinity, 0, 100, 50, 0, Infinity, 100, Infinity);
           ctx.drawImage(red, Infinity, 0, 100, 50, 0, 0, Infinity, 50);
           ctx.drawImage(red, Infinity, 0, 100, 50, 0, 0, Infinity, Infinity);
           ctx.drawImage(red, Infinity, 0, 100, 50, 0, 0, 100, Infinity);
           ctx.drawImage(red, 0, Infinity, Infinity, 50, 0, 0, 100, 50);
           ctx.drawImage(red, 0, Infinity, Infinity, Infinity, 0, 0, 100, 50);
           ctx.drawImage(red, 0, Infinity, Infinity, Infinity, Infinity, 0, 100, 50);
           ctx.drawImage(red, 0, Infinity, Infinity, Infinity, Infinity, Infinity, 100, 50);
           ctx.drawImage(red, 0, Infinity, Infinity, Infinity, Infinity, Infinity, Infinity, 50);
           ctx.drawImage(red, 0, Infinity, Infinity, Infinity, Infinity, Infinity, Infinity, Infinity);
           ctx.drawImage(red, 0, Infinity, Infinity, Infinity, Infinity, Infinity, 100, Infinity);
           ctx.drawImage(red, 0, Infinity, Infinity, Infinity, Infinity, 0, Infinity, 50);
           ctx.drawImage(red, 0, Infinity, Infinity, Infinity, Infinity, 0, Infinity, Infinity);
           ctx.drawImage(red, 0, Infinity, Infinity, Infinity, Infinity, 0, 100, Infinity);
           ctx.drawImage(red, 0, Infinity, Infinity, Infinity, 0, Infinity, 100, 50);
           ctx.drawImage(red, 0, Infinity, Infinity, Infinity, 0, Infinity, Infinity, 50);
           ctx.drawImage(red, 0, Infinity, Infinity, Infinity, 0, Infinity, Infinity, Infinity);
           ctx.drawImage(red, 0, Infinity, Infinity, Infinity, 0, Infinity, 100, Infinity);
           ctx.drawImage(red, 0, Infinity, Infinity, Infinity, 0, 0, Infinity, 50);
           ctx.drawImage(red, 0, Infinity, Infinity, Infinity, 0, 0, Infinity, Infinity);
           ctx.drawImage(red, 0, Infinity, Infinity, Infinity, 0, 0, 100, Infinity);
           ctx.drawImage(red, 0, Infinity, Infinity, 50, Infinity, 0, 100, 50);
           ctx.drawImage(red, 0, Infinity, Infinity, 50, Infinity, Infinity, 100, 50);
           ctx.drawImage(red, 0, Infinity, Infinity, 50, Infinity, Infinity, Infinity, 50);
           ctx.drawImage(red, 0, Infinity, Infinity, 50, Infinity, Infinity, Infinity, Infinity);
           ctx.drawImage(red, 0, Infinity, Infinity, 50, Infinity, Infinity, 100, Infinity);
           ctx.drawImage(red, 0, Infinity, Infinity, 50, Infinity, 0, Infinity, 50);
           ctx.drawImage(red, 0, Infinity, Infinity, 50, Infinity, 0, Infinity, Infinity);
           ctx.drawImage(red, 0, Infinity, Infinity, 50, Infinity, 0, 100, Infinity);
           ctx.drawImage(red, 0, Infinity, Infinity, 50, 0, Infinity, 100, 50);
           ctx.drawImage(red, 0, Infinity, Infinity, 50, 0, Infinity, Infinity, 50);
           ctx.drawImage(red, 0, Infinity, Infinity, 50, 0, Infinity, Infinity, Infinity);
           ctx.drawImage(red, 0, Infinity, Infinity, 50, 0, Infinity, 100, Infinity);
           ctx.drawImage(red, 0, Infinity, Infinity, 50, 0, 0, Infinity, 50);
           ctx.drawImage(red, 0, Infinity, Infinity, 50, 0, 0, Infinity, Infinity);
           ctx.drawImage(red, 0, Infinity, Infinity, 50, 0, 0, 100, Infinity);
           ctx.drawImage(red, 0, Infinity, 100, Infinity, 0, 0, 100, 50);
           ctx.drawImage(red, 0, Infinity, 100, Infinity, Infinity, 0, 100, 50);
           ctx.drawImage(red, 0, Infinity, 100, Infinity, Infinity, Infinity, 100, 50);
           ctx.drawImage(red, 0, Infinity, 100, Infinity, Infinity, Infinity, Infinity, 50);
           ctx.drawImage(red, 0, Infinity, 100, Infinity, Infinity, Infinity, Infinity, Infinity);
           ctx.drawImage(red, 0, Infinity, 100, Infinity, Infinity, Infinity, 100, Infinity);
           ctx.drawImage(red, 0, Infinity, 100, Infinity, Infinity, 0, Infinity, 50);
           ctx.drawImage(red, 0, Infinity, 100, Infinity, Infinity, 0, Infinity, Infinity);
           ctx.drawImage(red, 0, Infinity, 100, Infinity, Infinity, 0, 100, Infinity);
           ctx.drawImage(red, 0, Infinity, 100, Infinity, 0, Infinity, 100, 50);
           ctx.drawImage(red, 0, Infinity, 100, Infinity, 0, Infinity, Infinity, 50);
           ctx.drawImage(red, 0, Infinity, 100, Infinity, 0, Infinity, Infinity, Infinity);
           ctx.drawImage(red, 0, Infinity, 100, Infinity, 0, Infinity, 100, Infinity);
           ctx.drawImage(red, 0, Infinity, 100, Infinity, 0, 0, Infinity, 50);
           ctx.drawImage(red, 0, Infinity, 100, Infinity, 0, 0, Infinity, Infinity);
           ctx.drawImage(red, 0, Infinity, 100, Infinity, 0, 0, 100, Infinity);
           ctx.drawImage(red, 0, Infinity, 100, 50, Infinity, 0, 100, 50);
           ctx.drawImage(red, 0, Infinity, 100, 50, Infinity, Infinity, 100, 50);
           ctx.drawImage(red, 0, Infinity, 100, 50, Infinity, Infinity, Infinity, 50);
           ctx.drawImage(red, 0, Infinity, 100, 50, Infinity, Infinity, Infinity, Infinity);
           ctx.drawImage(red, 0, Infinity, 100, 50, Infinity, Infinity, 100, Infinity);
           ctx.drawImage(red, 0, Infinity, 100, 50, Infinity, 0, Infinity, 50);
           ctx.drawImage(red, 0, Infinity, 100, 50, Infinity, 0, Infinity, Infinity);
           ctx.drawImage(red, 0, Infinity, 100, 50, Infinity, 0, 100, Infinity);
           ctx.drawImage(red, 0, Infinity, 100, 50, 0, Infinity, 100, 50);
           ctx.drawImage(red, 0, Infinity, 100, 50, 0, Infinity, Infinity, 50);
           ctx.drawImage(red, 0, Infinity, 100, 50, 0, Infinity, Infinity, Infinity);
           ctx.drawImage(red, 0, Infinity, 100, 50, 0, Infinity, 100, Infinity);
           ctx.drawImage(red, 0, Infinity, 100, 50, 0, 0, Infinity, 50);
           ctx.drawImage(red, 0, Infinity, 100, 50, 0, 0, Infinity, Infinity);
           ctx.drawImage(red, 0, Infinity, 100, 50, 0, 0, 100, Infinity);
           ctx.drawImage(red, 0, 0, Infinity, Infinity, 0, 0, 100, 50);
           ctx.drawImage(red, 0, 0, Infinity, Infinity, Infinity, 0, 100, 50);
           ctx.drawImage(red, 0, 0, Infinity, Infinity, Infinity, Infinity, 100, 50);
           ctx.drawImage(red, 0, 0, Infinity, Infinity, Infinity, Infinity, Infinity, 50);
           ctx.drawImage(red, 0, 0, Infinity, Infinity, Infinity, Infinity, Infinity, Infinity);
           ctx.drawImage(red, 0, 0, Infinity, Infinity, Infinity, Infinity, 100, Infinity);
           ctx.drawImage(red, 0, 0, Infinity, Infinity, Infinity, 0, Infinity, 50);
           ctx.drawImage(red, 0, 0, Infinity, Infinity, Infinity, 0, Infinity, Infinity);
           ctx.drawImage(red, 0, 0, Infinity, Infinity, Infinity, 0, 100, Infinity);
           ctx.drawImage(red, 0, 0, Infinity, Infinity, 0, Infinity, 100, 50);
           ctx.drawImage(red, 0, 0, Infinity, Infinity, 0, Infinity, Infinity, 50);
           ctx.drawImage(red, 0, 0, Infinity, Infinity, 0, Infinity, Infinity, Infinity);
           ctx.drawImage(red, 0, 0, Infinity, Infinity, 0, Infinity, 100, Infinity);
           ctx.drawImage(red, 0, 0, Infinity, Infinity, 0, 0, Infinity, 50);
           ctx.drawImage(red, 0, 0, Infinity, Infinity, 0, 0, Infinity, Infinity);
           ctx.drawImage(red, 0, 0, Infinity, Infinity, 0, 0, 100, Infinity);
           ctx.drawImage(red, 0, 0, Infinity, 50, Infinity, 0, 100, 50);
           ctx.drawImage(red, 0, 0, Infinity, 50, Infinity, Infinity, 100, 50);
           ctx.drawImage(red, 0, 0, Infinity, 50, Infinity, Infinity, Infinity, 50);
           ctx.drawImage(red, 0, 0, Infinity, 50, Infinity, Infinity, Infinity, Infinity);
           ctx.drawImage(red, 0, 0, Infinity, 50, Infinity, Infinity, 100, Infinity);
           ctx.drawImage(red, 0, 0, Infinity, 50, Infinity, 0, Infinity, 50);
           ctx.drawImage(red, 0, 0, Infinity, 50, Infinity, 0, Infinity, Infinity);
           ctx.drawImage(red, 0, 0, Infinity, 50, Infinity, 0, 100, Infinity);
           ctx.drawImage(red, 0, 0, Infinity, 50, 0, Infinity, 100, 50);
           ctx.drawImage(red, 0, 0, Infinity, 50, 0, Infinity, Infinity, 50);
           ctx.drawImage(red, 0, 0, Infinity, 50, 0, Infinity, Infinity, Infinity);
           ctx.drawImage(red, 0, 0, Infinity, 50, 0, Infinity, 100, Infinity);
           ctx.drawImage(red, 0, 0, Infinity, 50, 0, 0, Infinity, 50);
           ctx.drawImage(red, 0, 0, Infinity, 50, 0, 0, Infinity, Infinity);
           ctx.drawImage(red, 0, 0, Infinity, 50, 0, 0, 100, Infinity);
           ctx.drawImage(red, 0, 0, 100, Infinity, Infinity, 0, 100, 50);
           ctx.drawImage(red, 0, 0, 100, Infinity, Infinity, Infinity, 100, 50);
           ctx.drawImage(red, 0, 0, 100, Infinity, Infinity, Infinity, Infinity, 50);
           ctx.drawImage(red, 0, 0, 100, Infinity, Infinity, Infinity, Infinity, Infinity);
           ctx.drawImage(red, 0, 0, 100, Infinity, Infinity, Infinity, 100, Infinity);
           ctx.drawImage(red, 0, 0, 100, Infinity, Infinity, 0, Infinity, 50);
           ctx.drawImage(red, 0, 0, 100, Infinity, Infinity, 0, Infinity, Infinity);
           ctx.drawImage(red, 0, 0, 100, Infinity, Infinity, 0, 100, Infinity);
           ctx.drawImage(red, 0, 0, 100, Infinity, 0, Infinity, 100, 50);
           ctx.drawImage(red, 0, 0, 100, Infinity, 0, Infinity, Infinity, 50);
           ctx.drawImage(red, 0, 0, 100, Infinity, 0, Infinity, Infinity, Infinity);
           ctx.drawImage(red, 0, 0, 100, Infinity, 0, Infinity, 100, Infinity);
           ctx.drawImage(red, 0, 0, 100, Infinity, 0, 0, Infinity, 50);
           ctx.drawImage(red, 0, 0, 100, Infinity, 0, 0, Infinity, Infinity);
           ctx.drawImage(red, 0, 0, 100, Infinity, 0, 0, 100, Infinity);
           ctx.drawImage(red, 0, 0, 100, 50, Infinity, Infinity, 100, 50);
           ctx.drawImage(red, 0, 0, 100, 50, Infinity, Infinity, Infinity, 50);
           ctx.drawImage(red, 0, 0, 100, 50, Infinity, Infinity, Infinity, Infinity);
           ctx.drawImage(red, 0, 0, 100, 50, Infinity, Infinity, 100, Infinity);
           ctx.drawImage(red, 0, 0, 100, 50, Infinity, 0, Infinity, 50);
           ctx.drawImage(red, 0, 0, 100, 50, Infinity, 0, Infinity, Infinity);
           ctx.drawImage(red, 0, 0, 100, 50, Infinity, 0, 100, Infinity);
           ctx.drawImage(red, 0, 0, 100, 50, 0, Infinity, Infinity, 50);
           ctx.drawImage(red, 0, 0, 100, 50, 0, Infinity, Infinity, Infinity);
           ctx.drawImage(red, 0, 0, 100, 50, 0, Infinity, 100, Infinity);
           ctx.drawImage(red, 0, 0, 100, 50, 0, 0, Infinity, Infinity);
           verify(Helper.comparePixel(ctx, 50,25, 0,255,0,255));

       }

       function test_negative() {
           var ctx = canvas.getContext('2d');
           ctx.reset();

           ctx.fillStyle = '#f00';
           ctx.fillRect(0, 0, 100, 50);
           ctx.drawImage('ggrr-256x256.png', 100, 78, 50, 50, 0, 50, 50, -50);
           ctx.drawImage('ggrr-256x256.png', 100, 128, 50, -50, 100, 50, -50, -50);
//           verify(Helper.comparePixel(ctx, 1,1, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 1,48, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 98,1, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 98,48, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 48,1, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 48,48, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 51,1, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 51,48, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 25,25, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 75,25, 0,255,0,255,2));

           ctx.reset();
           ctx.fillStyle = '#f00';
           ctx.fillRect(0, 0, 100, 50);
           ctx.drawImage('ggrr-256x256.png', 0, 178, 50, -100, 0, 0, 50, 100);
           ctx.drawImage('ggrr-256x256.png', 0, 78, 50, 100, 50, 100, 50, -100);
//           verify(Helper.comparePixel(ctx, 1,1, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 1,48, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 98,1, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 98,48, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 48,1, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 48,48, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 51,1, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 51,48, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 25,25, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 75,25, 0,255,0,255,2));

           ctx.reset();
           ctx.fillStyle = '#f00';
           ctx.fillRect(0, 0, 100, 50);
           ctx.drawImage('ggrr-256x256.png', 100, 78, -100, 50, 0, 0, 50, 50);
           ctx.drawImage('ggrr-256x256.png', 100, 128, -100, -50, 50, 0, 50, 50);
//           verify(Helper.comparePixel(ctx, 1,1, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 1,48, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 98,1, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 98,48, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 48,1, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 48,48, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 51,1, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 51,48, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 25,25, 0,255,0,255,2));
//           verify(Helper.comparePixel(ctx, 75,25, 0,255,0,255,2));

       }

       function test_canvas() {
           var ctx = canvas.getContext('2d');
           ctx.reset();
           var canvas2 = Qt.createQmlObject("import QtQuick 2.0; Canvas{renderTarget:Canvas.Image}", canvas);
           canvas2.width = 100;
           canvas2.height = 50;
           var ctx2 = canvas2.getContext('2d');
           ctx2.fillStyle = '#0f0';
           ctx2.fillRect(0, 0, 100, 50);

           ctx.fillStyle = '#f00';
           ctx.drawImage(canvas2, 0, 0);

           //verify(Helper.comparePixel(ctx, 0,0, 0,255,0,255,2));
           //verify(Helper.comparePixel(ctx, 99,0, 0,255,0,255,2));
           //verify(Helper.comparePixel(ctx, 0,49, 0,255,0,255,2));
           //verify(Helper.comparePixel(ctx, 99,49, 0,255,0,255,2));

       }

       function test_broken() {
           var ctx = canvas.getContext('2d');
           ctx.reset();
           var img = 'broken.png';
           verify(!img.complete);
           ctx.drawImage(img, 0, 0);
       }

       function test_alpha() {
           var ctx=canvas.getContext('2d');
           ctx.reset();
           ctx.fillStyle = '#0f0';
           ctx.fillRect(0, 0, 100, 50);
           ctx.globalAlpha = 0;
           ctx.drawImage('red.png', 0, 0);
           verify(Helper.comparePixel(ctx, 50,25, 0,255,0,255, 2));

       }
   }
}
