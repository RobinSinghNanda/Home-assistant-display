var cPI = 0;
var gC = [{}];
var nI = "mdi-dots-square";

function paY() {
    var ed = ace.edit("ed");
    try {
        var yamlString = ed.getValue();
        if (cPI >= 0 && cPI < gC.length) {
            if (yamlString == "") {
                gC[cPI] = {};
            } else {
                gC[cPI] = YAML.parse(yamlString);
            }
        }
    } catch (err) {}
}

function dH(ic, ti, pi, nuP) {
    var hE = "<div id='header'>";
    if (ic != "")
        hE += "<span class='iwrap'><span class='mdi " + ic + " ic'></span></span>";
    hE += "<span class='text'>" + ti + "</span>";
    hE += "<span style='float:right;' class='iwrap'><span class='mdi mdi-cog ic'></span></span>";
    hE += "<span style='float:right;' class='iwrap'><span class='mdi mdi-wifi-lock ic'></span></span>";
    if (nuP > 1) {
        hE += "<span class='pagei' style='float:right;width:" + (nuP * 20) + "px' ><svg style='width:" + (nuP * 20) + "px;height:40px;'>";
        for (var i = 0; i < nuP; i++) {
            hE += "<ellipse style='fill:#" + ((i == pi) ? "00a1f2" : "FFFFFF") + ";' cx='" + (i * 20 + 10) + "' cy='20' rx='3' ry='3'></ellipse>";
        }
        hE += "</svg></span>";
    }
    hE += "</div>";
    return hE;
}

function dER(eR) {
    if (typeof eR === "object") {
        if (eR["type"] == undefined) {
            if (eR["entity"] != undefined) {
                var ic = (eR["icon"] != undefined) ? gI(eR["icon"]) : nI;
                var na = (eR["name"] != undefined) ? eR["name"] : eR["entity"];
                return dSR(ic, na);
            } else {
                return "<div class='text erow'><p>ERROR:entity is not defined</p></div>";
            }
        } else if (eR["type"] == "custom:paper-buttons-row") {
            return dBR(eR);
        } else {
            return "<div class='erow text'><p>ERROR:entity " + eR["type"] + " type is not supported</p></div>";
        }
    } else {
        return dSR(nI, eR);
    }
}

function dB(c) {
    if (c["type"] == "entities") {
        var en = c["entities"];
        if (en != undefined) {
            if (Array.isArray(en)) {
                var e = "<div class='body'>";
                for (var i in en) {
                    e += dER(en[i]);
                }
                return e + "</div>";
            } else if (c["type"] == undefined) {} else {
                return "<div class='ecard text'>Error:entites is not defined</div>";
            }
        } else {
            return "<div class='ecard text'>Error:entites is not defined</div>";
        }
    } else if (c["type"] == undefined) {
        return "<div></div>";
    } else {
        return "<div class='ecard text'>Error:" + c["type"] + " type is not supported</div>";
    }

}

function dSR(ic, na) {
    return "<div class='row'><span class='iwrap'><span class='ic mdi " + ic + "' style='color:#376888;'></span></span><span class='text'>" + na + "</span><span class='sww'><img class='sw' src='sw-on.jpg'></span></div>";
}

function dBR(eR) {
    if (eR["buttons"] != undefined) {
        if (Array.isArray(eR["buttons"])) {
            var bE = "<div class='row' style='text-align:justify;'>";
            for (i in eR["buttons"]) {
                var ic = eR["buttons"][i]["icon"];
                bE += "<span class='iwrap' style='width:" + (100 / eR["buttons"].length) + "%;'><span class='ic mdi " + ((ic != undefined) ? gI(ic) : nI) + "' style='color:#376888;'></span></span>";
            }
            bE += "</div>";
            return bE;
        }
    }
}

function gI(p) {
    return nI + " " + p.replace("mdi:", "mdi-");
}

function drS() {
    if (cPI < 0 || cPI >= gC.length)
        return;
    var c = gC[cPI];
    if (c == undefined)
        return;
    var sE = eb("screen");
    sE.innerHTML = dH((c["icon"] != undefined) ? gI(c["icon"]) : "", (c["title"] != undefined) ? c["title"] : "", cPI, gC.length) + dB(c);
}

function ldB() {
    var hs = "";
    for (var cardNum in gC) {
        var cardId = parseInt(cardNum) + 1;
        var cardStr = "card" + cardId;
        var checked = (cardId == cPI + 1) ? "checked" : "";
        hs += "<input id='" + cardStr + "' type='radio' name='cS' value='" + cardStr + "' " + checked + "/>";
        hs += "<label for='" + cardStr + "' onclick='sC(" + cardId + ")'>" + cardId + "</label>";
    }
    eb("cas").innerHTML = hs;
}

function oLC() {
    if(!eb("ed")) {
        return;
    }
    rld();
    ace.config.set('basePath', 'https://cdnjs.cloudflare.com/ajax/libs/ace/1.4.12/');
    var ed = ace.edit("ed");
    ed.setTheme("ace/theme/chrome");
    ed.session.setMode("ace/mode/yaml");
    ed.getSession().on('change', function() {
        paY();
        drS();
    });
    ed.renderer.setShowGutter(false);
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            gC = JSON.parse(this.responseText);
            rld();
        }
    };
    xmlhttp.open("GET", "/cards.json", true);
    xmlhttp.send();
}

function rC() {
    var c = [];
    var n = 0;
    if (gC.length == 1) {
        gC[0] = {};
        rld();
        return;
    }
    for (var i = 0; i < gC.length; i++) {
        if (i != cPI) {
            c[n] = gC[i];
            n++;
        }
    }
    gC = c;
    if (cPI > 0) {
        cPI--;
    }
    rld();
}

function aC() {
    if (gC.length>=5)
        return;
    gC[gC.length] = {};
    rld();
}

function rld() {
    ldY();
    ldB();
    drS();
}

function sC(n) {
    if (n > 0 && n <= gC.length) {
        cPI = n - 1;
        rld();
    }
}

function ldY() {
    if (gC.length != 0 && cPI >= 0 && cPI < gC.length) {
        var yS = YAML.stringify(gC[cPI], 5);
        var ed = ace.edit("ed");
        if (yS == "{}") {
            ed.setValue("", -1);
        } else {
            ed.setValue(yS, -1);
        }
    }
}

function svC() {
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.open("POST", "/ca?save=1");
    xmlhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.open();
            document.write(xmlhttp.responseText);
            document.close();
        }
    };
    xmlhttp.setRequestHeader("Content-Type", "application/json");
    xmlhttp.send(JSON.stringify(gC));
}
wl(oLC);