var Clay = require('@rebble/clay');
var clayConfig = require('./config');
var clay = new Clay(clayConfig);

// ---------- Phone Battery ----------
// We need a Promise polyfill if the Pebble JS runtime doesn't provide it.
// Most modern runtimes do, but this is safe.
if (typeof Promise === 'undefined') {
  // A minimal polyfill (only what we need)
  window.Promise = function(executor) {
    var self = this;
    self._state = 'pending';
    self._value = undefined;
    self._callbacks = [];
    function resolve(value) {
      if (self._state !== 'pending') return;
      self._state = 'fulfilled';
      self._value = value;
      self._callbacks.forEach(function(cb) { cb.onFulfilled(value); });
    }
    function reject(reason) {
      if (self._state !== 'pending') return;
      self._state = 'rejected';
      self._value = reason;
      self._callbacks.forEach(function(cb) { cb.onRejected(reason); });
    }
    executor(resolve, reject);
  };
  Promise.prototype.then = function(onFulfilled, onRejected) {
    var self = this;
    return new Promise(function(resolve, reject) {
      function handle() {
        if (self._state === 'fulfilled') {
          if (typeof onFulfilled === 'function') {
            try { resolve(onFulfilled(self._value)); } catch (e) { reject(e); }
          } else { resolve(self._value); }
        } else if (self._state === 'rejected') {
          if (typeof onRejected === 'function') {
            try { resolve(onRejected(self._value)); } catch (e) { reject(e); }
          } else { reject(self._value); }
        } else {
          self._callbacks.push({ onFulfilled: function(v) { handle(); }, onRejected: function(r) { handle(); } });
        }
      }
      handle();
    });
  };
}

// Send phone battery to watch
function sendPhoneBattery(level, charging) {
  Pebble.sendAppMessage({
    'PhoneBattLevel': level,
    'PhoneBattCharging': charging ? 1 : 0
  }, function() {
    console.log('Phone battery sent: ' + level + '%');
  }, function(e) {
    console.error('Failed to send phone battery: ' + e);
  });
}

// Set up battery monitoring
function initPhoneBattery() {
  if (!navigator.getBattery) {
    console.warn('navigator.getBattery not available – phone battery won\'t be shown.');
    return;
  }
  navigator.getBattery().then(function(battery) {
    // Send initial value
    sendPhoneBattery(Math.floor(battery.level * 100), battery.charging);
    // Listen for changes
    battery.addEventListener('levelchange', function() {
      sendPhoneBattery(Math.floor(battery.level * 100), battery.charging);
    });
    battery.addEventListener('chargingchange', function() {
      sendPhoneBattery(Math.floor(battery.level * 100), battery.charging);
    });
  }).catch(function(err) {
    console.error('Error getting battery: ' + err);
  });
}

// Optionally respond to watch requests for phone battery
Pebble.addEventListener('appmessage', function(e) {
  if (e.payload && e.payload.QUERY_PHONE_BATT) {
    // If the watch asks, resend the latest known value.
    // We don't store it, so we just query again.
    if (navigator.getBattery) {
      navigator.getBattery().then(function(battery) {
        sendPhoneBattery(Math.floor(battery.level * 100), battery.charging);
      });
    }
  }
});

// Initialize phone battery when the watchface is ready
Pebble.addEventListener('ready', function() {
  initPhoneBattery();
});

// ---------- Clay already handles configuration ----------
// We don't need to override showConfiguration/webviewclosed
// because Clay does it for us. But if you need to do extra
// config handling, you can add listeners here.