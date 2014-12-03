self.onmessage = function(a) {
	switch (a.data.name) {
	case "M_Initialize":
		m = new q;
		this.postMessage({
			name: "M_Initialize_ACK",
			state: r(),
			board: t(),
			hand: u()
		});
		break;
	case "M_NewGame":
		aa(m);
		this.postMessage({
			name: "M_NewGame_ACK",
			state: r(),
			board: t(),
			hand: u()
		});
		break;
	case "M_MakeMove":
		m.M = a.data.thinkingTime;
		a = m;
		var c, b;
		for (c = v; c <= w; c++) for (b = 0; b < y; b++) - 1 < a.u[c][b].height && a.u[c][b].height--;
		a.C = 0;
		a.L = (new Date).getTime() + 1E3 * a.M;
		c = 1;
		for (a.abort = !1; !1 == a.abort;) b = ba(a, 0, c, -z, z), !1 == a.abort && (b > ca || b < -ca) && (a.abort = !0), c += 1;
		a.N = Math.floor(a.C / a.M / 1E3);
		c = [];
		b = a.u[a.c][a.f % y];
		for (var d = {}; null != b && 0 <= b.height && b.key == a.f;) {
			d[b.key.toString()] = b;
			if (0 > da(a).indexOf(b.A)) break;
			c.push(b);
			B(a, b.A);
			b = a.u[a.c][a.f % y];
			if (null == b.key || null != d[b.key.toString()]) break
		}
		for (b = c.length - 1; 0 <= b; b--) C(a, c[b].A);
		for (b = 0; b < c.length; b++);
		null != c && 0 < c.length && (B(a, c[0].A), ea(a, c[0].A));
		this.postMessage({
			name: "M_MakeMove_ACK",
			state: r(),
			board: t(),
			hand: u(),
			move: fa(),
			knodes: m.N
		});
		break;
	case "M_PlayUserMove":
		var g = a.data.fromRow,
			f = a.data.fromColumn;
		c = a.data.toRow;
		b = a.data.toColumn;
		var d = a.data.captureRow,
			k = a.data.captureColumn;
		a = m;
		if (0 > g || 0 > f) a: {
			g = D(a, 0);
			for (f = 0; f < g; f++) {
				var e = a.h[f];
				if (a.q(e) == E && F(a, e) == c && G(a, e) == b) if (0 <= d) {
					if (!0 == (0 != (e & I)) && (a.b[e >> 24].g >> 4 & 15) == d && (a.b[e >> 24].g & 15) == k) {
						B(a, e);
						c = e;
						break a
					}
				} else if (!1 == (0 != (e & I))) {
					B(a, e);
					c = e;
					break a
				}
			}
			throw Error("No move like that found");
		} else a: {
			for (var e = D(a, 0), h = 0; h < e; h++) {
				var l = a.h[h];
				if (a.q(l) != E && (a.b[l >> 8 & 255].g >> 4 & 15) == g && (a.b[l >> 8 & 255].g & 15) == f && F(a, l) == c && G(a, l) == b) if (0 <= d) {
					if (!0 == (0 != (l & I)) && (a.b[l >> 24].g >> 4 & 15) == d && (a.b[l >> 24].g & 15) == k) {
						B(a, l);
						c = l;
						break a
					}
				} else if (!1 == (0 != (l & I))) {
					B(a, l);
					c = l;
					break a
				}
			}
			throw Error("No move like that found");
		}
		ea(a, c);
		this.postMessage({
			name: "M_PlayUserMove_ACK",
			state: r(),
			board: t(),
			hand: u()
		});
		break;
	case "M_StartUserMove":
		this.postMessage({
			name: "M_StartUserMove_ACK",
			state: r(),
			board: t(),
			hand: u(),
			side: m.c,
			inserts: ga(),
			origins: ha()
		});
		break;
	case "M_GetCaptureLocations":
		0 > a.data.fromRow ? this.postMessage({
			name: "M_GetCaptureLocations_ACK",
			side: m.c,
			captures: ia(a.data.toRow, a.data.toColumn)
		}) : this.postMessage({
			name: "M_GetCaptureLocations_ACK",
			side: m.c,
			captures: ja(a.data.fromRow, a.data.fromColumn, a.data.toRow, a.data.toColumn)
		});
		break;
	case "M_GetMoveDestinations":
		this.postMessage({
			name: "M_GetMoveDestinations_ACK",
			side: m.c,
			destinations: ka(a.data.row, a.data.column)
		});
		break;
	case "M_TakeBack":
		a = m, null != a.w && 0 < a.w.length && C(a, a.w.pop()), this.postMessage({
			name: "M_TakeBack_ACK",
			state: r(),
			board: t(),
			hand: u()
		})
	}
};
var m;

function J(a, c, b) {
	this.g = a;
	this.s = Array(c);
	this.a = Array(2);
	this.D = this.H = 0;
	this.l = K;
	this.p = -1;
	this.i = b
}
J.prototype.l;
J.prototype.p;
J.prototype.g;
J.prototype.i;
J.prototype.s;
J.prototype.D = 0;
J.prototype.a;
J.prototype.H;
function L(a, c, b) {
	this.b = Array(3);
	this.b[0] = a;
	this.b[1] = c;
	this.b[2] = b;
	this.t = Array(3);
	this.t[v] = this.t[w] = 0;
	this.t[K] = 3;
	this.d = this.t[v] << 8 | this.t[w] << 4 | this.t[K]
}
function M(a, c, b) {
	a.t[c]--;
	a.t[b]++;
	a.d = a.t[v] << 8 | a.t[w] << 4 | a.t[K]
}
L.prototype.b;
L.prototype.t;
L.prototype.d;

function N() {
	this.height = -1
}
N.prototype.key;
N.prototype.height;
N.prototype.G;
N.prototype.I;
N.prototype.A;
function q() {
	aa(this)
}
q.prototype.C;
q.prototype.L;
q.prototype.M = 1;
q.prototype.N = 0;

function ba(a, c, b, d, g) {
	if (c == b) return la(a, c, d, g);
	a.C++;
	if (0 == (a.C & 65535) && (new Date).getTime() > a.L) return a.abort = !0, 0;
	var f = d,
		k = b - c,
		e = a.u[a.c][a.f % y],
		h = ma;
	if (e.key == a.f) {
		if (e.height >= k) {
			if (e.G == na) return e.I;
			e.G == oa ? d = Math.max(d, e.I) : e.G == pa && (g = Math.min(g, e.I))
		}
		h = e.A
	}
	if (!(d >= g)) {
		if (3 > a.o[a.c] + a.n[a.c]) return -z + c;
		var l = D(a, c);
		if (0 == l) return -z + c;
		var p = g,
			x;
		for (x = 0; x < l; x++) {
			qa(a, c, l, x, h);
			var A = a.h[O * c + x];
			B(a, A);
			p = -ba(a, c + 1, b, -p, -d);
			0 < x && d < p && p < g && (p = -ba(a, c + 1, b, -g, -d));
			C(a, A);
			if (!0 == a.abort) return 0;
			if (p > d && (h = A, a.history[A >> 16 & 255] += k << k, d = p, d >= g)) break;
			p = d + 1
		}
	}
	h != ma && k >= e.height && (e.G = d <= f ? pa : d >= g ? oa : na, e.A = h, e.height = k, e.I = d, e.key = a.f);
	return d
}
function qa(a, c, b, d, g) {
	c *= O;
	for (var f = -1, k = -1, e = d; e < b; e++) {
		if (a.h[c + e] == g) {
			k = e;
			break
		}
		a.m[c + e] > f && (f = a.m[c + e], k = e)
	}
	0 <= k && (b = a.h[c + k], a.h[c + k] = a.h[c + d], a.h[c + d] = b, b = a.m[c + k], a.m[c + k] = a.m[c + d], a.m[c + d] = b)
}

function ra(a) {
	var c = a.o[v] + a.n[v] - (a.o[w] + a.n[w]),
		b;
	a.F[v] = a.F[w] = 0;
	for (b = v; b <= w; b++) for (var d = 0; 9 > d; d++) {
		var g = a.k[9 * b + d];
		if (0 <= g) for (var g = a.b[g], f = 0; f < g.s.length; f++) n = g.s[f], n.l == K && a.F[b]++
	}
	b = a.F[v] - a.F[w];
	d = a.e[sa] - a.e[ta];
	c = 1 * c + 2 * b + 10 * d;
	a.c == w && (c *= -1);
	return c
}
q.prototype.F = [];

function la(a, c, b, d) {
	a.C++;
	if (0 == (a.C & 65535) && (new Date).getTime() > a.L) return a.abort = !0, 0;
	if (c == P) return ra(a);
	var g = ra(a);
	if (g >= d) return d;
	b < g && (b = g);
	var f;
	f = O * c;
	if (0 < a.n[a.c]) for (var k = 0; k < a.b.length; k++) {
		var e = a.b[k];
		if (e.l == K) {
			var h = E | e.i << 16 | ua << 8;
			if (!0 == va(a.b[k], a.c)) {
				for (var l = 0, p = 0; 9 > p; p++) {
					var x = 9 * a.j + p,
						e = a.k[x]; - 1 != e && !1 == Q(a.b[e], a.j) && (a.h[f] = h | e << 24 | I, a.m[f++] = R, l++)
				}
				if (0 == l) for (p = 0; 9 > p; p++) x = 9 * a.j + p, e = a.k[x], -1 != e && (a.h[f] = h | e << 24 | I, a.m[f++] = R)
			}
		}
	} else if (3 < a.o[a.c]) for (k = 0; 9 > k; k++) {
		if (l = a.k[9 * a.c + k], -1 != l) for (var h = a.b[l], A = 0; A < h.s.length; A++) {
			var H = h.s[A];
			if (H.l == K && !0 == S(h, H, a.c)) {
				for (p = l = 0; 9 > p; p++) x = 9 * a.j + p, e = a.k[x], -1 != e && !1 == Q(a.b[e], a.j) && (a.h[f] = e << 24 | H.i << 16 | h.i << 8 | I | T, a.m[f++] = R, l++);
				if (0 == l) for (p = 0; 9 > p; p++) x = 9 * a.j + p, varcaptureIndex = a.k[x], -1 != e && (a.h[f] = e << 24 | H.i << 16 | h.i << 8 | I | T, a.m[f++] = R)
			}
		}
	} else for (k = 0; 9 > k; k++) if (l = a.k[9 * a.c + k], -1 != l) for (h = a.b[l], A = 0; A < a.b.length; A++) if (H = a.b[A], H.l == K && !0 == S(h, H, a.c)) {
		for (p = l = 0; 9 > p; p++) x = 9 * a.j + p, e = a.k[x], -1 != e && !1 == Q(a.b[e], a.j) && (a.h[f] = e << 24 | H.i << 16 | h.i << 8 | I | U, a.m[f++] = R, l++);
		if (0 == l) for (p = 0; 9 > p; p++) x = 9 * a.j + p, e = a.k[x], -1 != e && (a.h[f] = e << 24 | H.i << 16 | h.i << 8 | I | U, a.m[f++] = R)
	}
	f -= O * c;
	if (0 == f) {
		a: if (0 < a.n[a.c] || 3 == a.o[a.c]) a = !0;
		else {
			if (3 < a.o[a.c]) for (b = 0; 9 > b; b++) if (d = a.k[9 * a.c + b], -1 != d) for (d = a.b[d], f = 0; f < d.s.length; f++) if (d.s[f].l == K) {
				a = !0;
				break a
			}
			a = !1
		}
		return !1 == a ? -z + c : g
	}
	for (g = 0; g < f; g++) {
		qa(a, c, f, g);
		k = a.h[O * c + g];
		B(a, k);
		e = -la(a, c + 1, -d, -b);
		C(a, k);
		if (!0 == a.abort) return 0;
		if (e > b) {
			if (e >= d) return d;
			b = e
		}
	}
	return b
}

function da(a) {
	for (var c = [], b = D(a, 0), d = 0; d < b; d++) c.push(a.h[d]);
	return c
}

function D(a, c) {
	var b, d, g, f, k, e, h, l = O * c;
	if (0 < a.n[a.c]) {
		for (b = 0; b < a.b.length; b++) if (f = a.b[b], f.l == K) if (d = E | f.i << 16 | ua << 8, !0 == va(a.b[b], a.c)) {
			for (f = k = 0; 9 > f; f++) e = 9 * a.j + f, e = a.k[e], -1 != e && !1 == Q(a.b[e], a.j) && (a.h[l] = d | e << 24 | I, a.m[l++] = R, k++);
			if (0 == k) for (f = 0; 9 > f; f++) e = 9 * a.j + f, e = a.k[e], -1 != e && (a.h[l] = d | e << 24 | I, a.m[l++] = R)
		} else a.h[l] = d, a.m[l++] = a.history[d >> 16 & 255];
		return l - O * c
	}
	if (3 < a.o[a.c]) {
		for (b = 0; 9 > b; b++) if (f = a.k[9 * a.c + b], -1 != f) for (d = a.b[f], g = 0; g < d.s.length; g++) if (h = d.s[g], h.l == K) if (!0 == S(d, h, a.c)) {
			for (f = k = 0; 9 > f; f++) e = 9 * a.j + f, e = a.k[e], -1 != e && !1 == Q(a.b[e], a.j) && (a.h[l] = e << 24 | h.i << 16 | d.i << 8 | I | T, a.m[l++] = R, k++);
			if (0 == k) for (f = 0; 9 > f; f++) e = 9 * a.j + f, e = a.k[e], -1 != e && (a.h[l] = e << 24 | h.i << 16 | d.i << 8 | I | T, a.m[l++] = R)
		} else a.h[l] = h.i << 16 | d.i << 8 | T, a.m[l++] = a.history[d.i, h.i];
		return l - O * c
	}
	for (b = 0; 9 > b; b++) if (f = a.k[9 * a.c + b], -1 != f) for (d = a.b[f], g = 0; g < a.b.length; g++) if (h = a.b[g], h.l == K) if (!0 == S(d, h, a.c)) {
		for (f = k = 0; 9 > f; f++) e = 9 * a.j + f, e = a.k[e], -1 != e && !1 == Q(a.b[e], a.j) && (a.h[l] = e << 24 | h.i << 16 | d.i << 8 | I | U, a.m[l++] = R, k++);
		if (0 == k) for (f = 0; 9 > f; f++) e = 9 * a.j + f, e = a.k[e], -1 != e && (a.h[l] = e << 24 | h.i << 16 | d.i << 8 | I | U, a.m[l++] = R)
	} else a.h[l] = h.i << 16 | d.i << 8 | U, a.m[l++] = a.history[d.i, h.i];
	return l - O * c
}
function va(a, c) {
	return c == v ? a.a[0].d == V || a.a[1].d == V ? !0 : !1 : a.a[0].d == W || a.a[1].d == W ? !0 : !1
}

function S(a, c, b) {
	return b == v ? c.a[0].d == V && c.a[0].b[0] != a && c.a[0].b[1] != a && c.a[0].b[2] != a || c.a[1].d == V && c.a[1].b[0] != a && c.a[1].b[1] != a && c.a[1].b[2] != a ? !0 : !1 : c.a[0].d == W && c.a[0].b[0] != a && c.a[0].b[1] != a && c.a[0].b[2] != a || c.a[1].d == W && c.a[1].b[0] != a && c.a[1].b[1] != a && c.a[1].b[2] != a ? !0 : !1
}
function Q(a, c) {
	return c == v ? a.a[0].d == sa || a.a[1].d == sa ? !0 : !1 : a.a[0].d == ta || a.a[1].d == ta ? !0 : !1
}

function aa(a) {
	a.O = 0;
	a.b = [];
	a.J = 0;
	X(a, 0, 2);
	X(a, 3, 3);
	X(a, 6, 2);
	X(a, 17, 2);
	X(a, 19, 4);
	X(a, 21, 2);
	X(a, 34, 2);
	X(a, 35, 3);
	X(a, 36, 2);
	X(a, 48, 3);
	X(a, 49, 4);
	X(a, 50, 3);
	X(a, 52, 3);
	X(a, 53, 4);
	X(a, 54, 3);
	X(a, 66, 2);
	X(a, 67, 3);
	X(a, 68, 2);
	X(a, 81, 2);
	X(a, 83, 4);
	X(a, 85, 2);
	X(a, 96, 2);
	X(a, 99, 3);
	X(a, 102, 2);
	a.e = [];
	var c;
	for (c = 0; 1024 > c; c++) a.e[c] = 0;
	a.a = [];
	a.B = 0;
	Y(a, 0, 3, 6);
	Y(a, 17, 19, 21);
	Y(a, 34, 35, 36);
	Y(a, 48, 49, 50);
	Y(a, 52, 53, 54);
	Y(a, 66, 67, 68);
	Y(a, 81, 83, 85);
	Y(a, 96, 99, 102);
	Y(a, 0, 48, 96);
	Y(a, 17, 49, 81);
	Y(a, 34, 50, 66);
	Y(a, 3, 19, 35);
	Y(a, 67, 83, 99);
	Y(a, 36, 52, 68);
	Y(a, 21, 53, 85);
	Y(a, 6, 54, 102);
	a.o = Array(3);
	a.o[v] = a.o[w] = 0;
	a.o[K] = 24;
	a.k = Array(18);
	for (c = 0; c < a.k.length; c++) a.k[c] = -1;
	a.h = Array(O * P);
	a.m = Array(O * P);
	for (c = 0; c < O * P; c++) a.h[c] = a.m = 0;
	a.n = Array(2);
	a.n[0] = a.n[1] = 9;
	a.c = v;
	a.j = w;
	if (null == a.r) {
		var b = Math.max(Math.max(v, w), K);
		a.r = Array(b + 1);
		var d;
		for (c = 0; c < a.r.length; c++) a.r[c] = Array(a.b.length + 1);
		for (c = 0; c <= b; ++c) for (d = 0; d <= a.b.length; ++d) a.r[c][d] = 2147483648 * Math.random() >>> 0;
		a.u = Array(2);
		a.u[0] = Array(y);
		a.u[1] = Array(y);
		for (b = v; b <= w; b++) for (c = 0; c < y; c++) a.u[b][c] = new N;
		a.v = Array(2);
		a.v[0] = Array(10);
		a.v[1] = Array(10);
		for (b = v; b <= w; b++) for (c = 0; 9 >= c; c++) a.v[b][c] = 2147483648 * Math.random() >>> 0
	} else for (b = v; b <= w; b++) for (c = 0; c < y; c++) a.u[b][c].height = -1;
	c = a.v[v][a.n[v]];
	c ^= a.v[w][a.n[w]];
	for (b = 0; b < a.b.length; b++) d = a.b[b], d.l != K && (c ^= a.r[d.l][d.i]);
	a.f = c;
	a.w = [];
	a.K = [];
	a.history = Array(Z);
	for (b = 0; b < Z; b++) for (a.history[b] = Array(Z), c = 0; c < Z; c++) a.history[b][c] = 0
}
q.prototype.c;
q.prototype.j;
q.prototype.w = [];
q.prototype.K = [];

function ea(a, c) {
	a.w.push(c);
	a.K.push(void 0)
}
function fa() {
	var a = m;
	if (null == a.w || 0 == a.w.length) return null;
	var c = a.w[a.w.length - 1],
		b = {
			MoveType: a.q(c) == E ? "Insert" : "SlideOrFly"
		};
	a.q(c) != E && (b.FromSquare = {
		row: a.b[c >> 8 & 255].g >> 4 & 15,
		column: a.b[c >> 8 & 255].g & 15
	});
	b.ToSquare = {
		row: F(a, c),
		column: G(a, c)
	};
	b.IsCapture = 0 != (c & I);
	!0 == (0 != (c & I)) && (b.CaptureSquare = {
		row: a.b[c >> 24].g >> 4 & 15,
		column: a.b[c >> 24].g & 15
	});
	b.White = a.c == w;
	return b
}

function r() {
	var a = m;
	if (3 > a.o[v] + a.n[v]) return wa;
	if (3 > a.o[w] + a.n[w]) return xa;
	if (0 == da(a)) {
		if (a.c == v) return wa;
		if (a.c == w) return xa
	}
	for (var c = 0, b = 0; b < a.O; b++) a.K[b] == a.f && c++;
	return 3 <= c ? ya : a.c == w ? za : Aa
}
function X(a, c, b) {
	a.b[a.J] = new J(c, b, a.J);
	a.J++
}
q.prototype;
q.prototype.b;
q.prototype.J;

function Y(a, c, b, d) {
	var g = $(a, c),
		f = $(a, b);
	g.s[g.D++] = f;
	f.s[f.D++] = g;
	g = $(a, b);
	f = $(a, d);
	g.s[g.D++] = f;
	f.s[f.D++] = g;
	a.a[a.B] = new L($(a, c), $(a, b), $(a, d));
	c = $(a, c);
	g = a.a[a.B];
	c.a[c.H++] = g;
	b = $(a, b);
	c = a.a[a.B];
	b.a[b.H++] = c;
	d = $(a, d);
	b = a.a[a.B];
	d.a[d.H++] = b;
	a.e[a.a[a.B].d]++;
	a.B++
}
function $(a, c) {
	var b;
	for (b = 0; b < a.b.length; b++) {
		var d = a.b[b];
		if (d.g == c) return d
	}
	throw Error("Invalid Index");
}

function t() {
	for (var a = m, c = [], b = 0; b < Ba * Ca; b++) c[b] = K;
	for (b = 0; b < a.b.length; b++) {
		var d = a.b[b];
		d.l != K && (c[(d.g >> 4) * Ca + (d.g & 15)] = d.l)
	}
	return c
}
function u() {
	var a = m;
	return {
		White: a.n[v],
		Black: a.n[w]
	}
}
function ga() {
	for (var a = m, c = [], b = D(a, 0), d = 0; d < b; d++) {
		var g = a.h[d];
		if (a.q(g) == E) {
			var g = a.b[g >> 16 & 255],
				f = g.g & 15;
			c.push(g.g >> 4 & 15);
			c.push(f)
		}
	}
	return c
}

function ha() {
	for (var a = m, c = [], b = [], d = D(a, 0), g = 0; g < d; g++) {
		var f = a.h[g];
		if (a.q(f) == T || a.q(f) == U) if (f = a.b[f >> 8 & 255], 0 > b.indexOf(f.g)) {
			b.push(f.g);
			var k = f.g & 15;
			c.push(f.g >> 4 & 15);
			c.push(k)
		}
	}
	return c
}
function ka(a, c) {
	for (var b = m, d = [], g = [], f = D(b, 0), k = 0; k < f; k++) {
		var e = b.h[k];
		if (b.q(e) == T || b.q(e) == U) {
			var h = e >> 8 & 255,
				h = b.b[h],
				l = h.g & 15;
			a == (h.g >> 4 & 15) && c == l && (h = e >> 16 & 255, 0 > g.indexOf(h) && (g.push(h), h = b.b[h], e = h.g & 15, d.push(h.g >> 4 & 15), d.push(e)))
		}
	}
	return d
}

function ja(a, c, b, d) {
	for (var g = m, f = [], k = D(g, 0), e = 0; e < k; e++) {
		var h = g.h[e];
		if ((g.q(h) == T || g.q(h) == U) && !0 == (0 != (h & I)) && (g.b[h >> 8 & 255].g >> 4 & 15) == a && (g.b[h >> 8 & 255].g & 15) == c && F(g, h) == b && G(g, h) == d) {
			var h = g.b[h >> 24],
				l = h.g & 15;
			f.push(h.g >> 4 & 15);
			f.push(l)
		}
	}
	return f
}
function ia(a, c) {
	for (var b = m, d = [], g = D(b, 0), f = 0; f < g; f++) {
		var k = b.h[f];
		b.q(k) == E && !0 == (0 != (k & I)) && F(b, k) == a && G(b, k) == c && (index = k >> 24, s = b.b[index], k = s.g & 15, d.push(s.g >> 4 & 15), d.push(k))
	}
	return d
}
q.prototype.B;
q.prototype.e;
q.prototype.k;
q.prototype.n;
q.prototype.u;
q.prototype.f;
q.prototype.v;
q.prototype.r;

function B(a, c) {
	if (a.q(c) == E) {
		for (var b = c >> 16 & 255, d = a.b[b], g = 9 * a.c; 0 <= a.k[g];) g++;
		a.k[g] = b;
		d.p = g;
		a.o[a.c]++;
		a.e[d.a[0].d]--;
		a.e[d.a[1].d]--;
		M(d.a[0], K, a.c);
		M(d.a[1], K, a.c);
		a.e[d.a[0].d]++;
		a.e[d.a[1].d]++;
		d.l = a.c;
		a.f ^= a.v[a.c][a.n[a.c]];
		a.n[a.c]--;
		a.f ^= a.v[a.c][a.n[a.c]];
		a.f ^= a.r[a.c][b]
	} else d = c >> 8 & 255, b = c >> 16 & 255, a.f ^= a.r[a.c][d], a.f ^= a.r[a.c][b], d = a.b[d], b = a.b[b], a.e[d.a[0].d]--, a.e[d.a[1].d]--, M(d.a[0], d.l, K), M(d.a[1], d.l, K), a.e[d.a[0].d]++, a.e[d.a[1].d]++, a.e[b.a[0].d]--, a.e[b.a[1].d]--, M(b.a[0], K, a.c), M(b.a[1], K, a.c), a.e[b.a[0].d]++, a.e[b.a[1].d]++, b.p = d.p, a.k[b.p] = b.i, b.l = d.l, d.p = -1, d.l = K;
	!0 == (0 != (c & I)) && (b = c >> 24, d = a.b[b], a.f ^= a.r[a.j][b], a.k[d.p] = -1, d.p = -1, a.o[a.j]--, a.e[d.a[0].d]--, a.e[d.a[1].d]--, M(d.a[0], d.l, K), M(d.a[1], d.l, K), a.e[d.a[0].d]++, a.e[d.a[1].d]++, d.l = K);
	a.j = a.c;
	a.c ^= 1
}

function C(a, c) {
	var b, d;
	a.j = a.c;
	a.c ^= 1;
	a.q(c) == E ? (d = c >> 16 & 255, b = a.b[d], b.l = K, a.f ^= a.r[a.c][d], a.e[b.a[0].d]--, a.e[b.a[1].d]--, M(b.a[0], a.c, K), M(b.a[1], a.c, K), a.e[b.a[0].d]++, a.e[b.a[1].d]++, a.k[b.p] = -1, b.p = -1, a.o[a.c]--, a.f ^= a.v[a.c][a.n[a.c]], a.n[a.c]++, a.f ^= a.v[a.c][a.n[a.c]]) : (b = c >> 8 & 255, d = c >> 16 & 255, a.f ^= a.r[a.c][d], a.f ^= a.r[a.c][b], b = a.b[b], d = a.b[d], a.e[b.a[0].d]--, a.e[b.a[1].d]--, M(b.a[0], K, a.c), M(b.a[1], K, a.c), a.e[b.a[0].d]++, a.e[b.a[1].d]++, a.e[d.a[0].d]--, a.e[d.a[1].d]--, M(d.a[0], a.c, K), M(d.a[1], a.c, K), a.e[d.a[0].d]++, a.e[d.a[1].d]++, b.p = d.p, a.k[b.p] = b.i, b.l = a.c, d.p = -1, d.l = K);
	if (!0 == (0 != (c & I))) {
		var g;
		b = c >> 24;
		d = a.b[b];
		for (g = 9 * a.j; 0 <= a.k[g];) g++;
		a.k[g] = b;
		d.p = g;
		a.o[a.j]++;
		a.e[d.a[0].d]--;
		a.e[d.a[1].d]--;
		M(d.a[0], K, a.j);
		M(d.a[1], K, a.j);
		a.e[d.a[0].d]++;
		a.e[d.a[1].d]++;
		d.l = a.j;
		a.f ^= a.r[a.j][b]
	}
}
q.prototype.q = function(a) {
	return a & Da
};
function F(a, c) {
	return a.b[c >> 16 & 255].g >> 4 & 15
}
function G(a, c) {
	return a.b[c >> 16 & 255].g & 15
}
q.prototype.history;
var Z = 128,
	xa = 0,
	wa = 1,
	Aa = 2,
	za = 3,
	ya = 4,
	Ba = 7,
	Ca = 7,
	v = 0,
	w = 1,
	K = 2,
	P = 40,
	O = 648,
	y = 1E4,
	na = 0,
	pa = 1,
	oa = 2,
	E = 0,
	T = 1,
	U = 2,
	I = 4,
	Da = 3,
	ua = 112,
	z = 1E5,
	ca = 9E4,
	V = 513,
	sa = 768,
	W = 33,
	ta = 48,
	R = 1E7,
	ma = -1;