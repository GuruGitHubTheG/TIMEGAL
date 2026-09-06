var Clay = require('@rebble/clay');
var clayConfig = require('./config');
var clay = new Clay(clayConfig);

// ---------- Promise polyfill (if needed) ----------
if (typeof Promise === 'undefined') {
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
          self._callbacks.push({
            onFulfilled: function(v) { handle(); },
            onRejected: function(r) { handle(); }
          });
        }
      }
      handle();
    });
  };
}

// ---------- Phone Battery ----------
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

function initPhoneBattery() {
  if (!navigator.getBattery) {
    console.warn('navigator.getBattery not available – phone battery won\'t be shown.');
    return;
  }
  navigator.getBattery().then(function(battery) {
    sendPhoneBattery(Math.floor(battery.level * 100), battery.charging);
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

// ---------- Japanese Year ----------
function fetchJapaneseYear() {
  var year = new Date().getFullYear();
  var url = 'https://seireki.teraren.com/seireki/' + year + '.json';

  var xhr = new XMLHttpRequest();
  xhr.open('GET', url, true);
  xhr.setRequestHeader('Accept', 'application/json');
  xhr.onload = function() {
    if (xhr.status === 200) {
      try {
        var data = JSON.parse(xhr.responseText);
        if (data.wareki) {
          var parts = data.wareki.split('または');
          var latest = parts[parts.length - 1].trim();
          Pebble.sendAppMessage({
            'JapaneseYear': latest
          }, function() {
            console.log('Japanese year sent: ' + latest);
          }, function(e) {
            console.error('Failed to send Japanese year: ' + e);
          });
        }
      } catch(e) {
        console.error('Error parsing Japanese year response: ' + e);
      }
    } else {
      console.error('Japanese year fetch failed with status ' + xhr.status);
    }
  };
  xhr.onerror = function() {
    console.error('Network error fetching Japanese year');
  };
  xhr.send();
}

// ---------- AppMessage listener ----------
Pebble.addEventListener('appmessage', function(e) {
  // Handle watch request for phone battery
  if (e.payload && e.payload.QUERY_PHONE_BATT) {
    if (navigator.getBattery) {
      navigator.getBattery().then(function(battery) {
        sendPhoneBattery(Math.floor(battery.level * 100), battery.charging);
      });
    }
  }

  // Handle watch request for Japanese year
  if (e.payload && e.payload.QUERY_JAPANESE_YEAR) {
    fetchJapaneseYear();
  }
});

// ---------- Ready event ----------
Pebble.addEventListener('ready', function() {
  initPhoneBattery();
  // Fetch Japanese year once on startup (watch can also request it later)
  fetchJapaneseYear();
});