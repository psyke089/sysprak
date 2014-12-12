	function startMorris() {
		try {
			mm.newGameButton = $("#newgame"), mm.makeMoveButton = $("#makemove"), 
			mm.takeBackButton = $("#takeback"), worker = new Worker("/static/js/9mm/morrisengine.js"), 
			worker.onmessage = OnEngineMessage, worker.postMessage({
				name: "M_Initialize"
			})
		} catch (e) {
			window.alert("Sorry your browser does not have the\ncapabilities required to run this program.\n\n
				This program is optimized for the Chrome browser from Google.")
		}
	}
	function OnInsertComplete(e, t) {
		__fromRow = __fromColumn = -1, __toRow = e, __toColumn = t, worker.postMessage({
			name: "M_GetCaptureLocations",
			fromRow: -1,
			fromColumn: -1,
			toRow: e,
			toColumn: t
		})
	}
	function OnSlideOrFlyStarted(e, t) {
		worker.postMessage({
			name: "M_GetMoveDestinations",
			row: e,
			column: t
		})
	}
	function OnSlideOrFlyCompleted(e, t, a, o) {
		__fromRow = e, __fromColumn = t, __toRow = a, __toColumn = o, worker.postMessage({
			name: "M_GetCaptureLocations",
			fromRow: e,
			fromColumn: t,
			toRow: a,
			toColumn: o
		})
	}
	function OnCaptureCompleted(e, t, a, o, n, r) {
		worker.postMessage({
			name: "M_PlayUserMove",
			fromRow: e,
			fromColumn: t,
			toRow: a,
			toColumn: o,
			captureRow: n,
			captureColumn: r
		})
	}
	function enableButton(e, t) {
		t ? e.removeAttr("disabled") : e.attr("disabled", "")
	}
	function SetState(e) {
		state = e, state == THINKING || state == UNKNOWN ? (enableButton(mm.newGameButton, !1), enableButton(mm.makeMoveButton, !1), enableButton(mm.takeBackButton, !1)) : (enableButton(mm.newGameButton, !0), enableButton(mm.takeBackButton, !0), state == WHITE_MOVE || state == BLACK_MOVE ? enableButton(mm.makeMoveButton, !0) : enableButton(mm.makeMoveButton, !1));
		var t, a = document.getElementById("status"),
			o = document.getElementById("hint"),
			n = "";
		switch (state) {
		case UNKNOWN:
			t = "Initializing...", n = "Please wait while the A.I. is configured.";
			break;
		case WHITE_WIN:
			t = "White Wins", n = "Black has less than three pieces or no legal moves.";
			break;
		case BLACK_WIN:
			t = "Black Wins", n = "White has less than three pieces or no legal moves.";
			break;
		case WHITE_MOVE:
			t = "White To Move", n = lastHand.White > 0 ? "Move the top most piece from the white pile onto the board." : "Move any white piece to another location. Valid locations are highlighted once you select a piece.";
			break;
		case BLACK_MOVE:
			t = "Black To Move", n = lastHand.Black > 0 ? "Move the top most piece from the black pile onto the board." : "Move any black piece to another location. Valid locations are highlighted once you select a piece.";
			break;
		case DRAW:
			t = "The Game Is Drawn", n = "The position has been repeated three times therefore the game is drawn.";
			break;
		case THINKING:
			t = "Thinking...", n = "The computer is thinking abouts its move."
		}
		a.textContent = t, o.textContent = n, (state == WHITE_MOVE || state == BLACK_MOVE) && worker.postMessage({
			name: "M_StartUserMove"
		})
	}
	function OnMakeMove() {
		(state == WHITE_MOVE || state == BLACK_MOVE) && (SetState(THINKING), worker.postMessage({
			name: "M_MakeMove",
			thinkingTime: GetThinkingTime()
		}))
	}
	function OnTakeBack() {
		SetState(THINKING), worker.postMessage({
			name: "M_TakeBack"
		})
	}
	function OnNewGame() {
		confirm("Start a New Game?") && (SetState(THINKING), worker.postMessage({
			name: "M_NewGame"
		}))
	}
	function OnShowMoveCompleted() {
		SetState(pendingState)
	}
	function OnEngineMessage(e) {
		var t = e.data.name;
		switch (t) {
		case "M_Initialize_ACK":
			document.getElementById("newgame").onclick = OnNewGame, document.getElementById("makemove").onclick = OnMakeMove, document.getElementById("takeback").onclick = OnTakeBack;
			try {
				b = new MorrisBoard("container", 600, 600), b.AddListener("InsertCompleted", OnInsertComplete), b.AddListener("SlideOrFlyStarted", OnSlideOrFlyStarted), b.AddListener("SlideOrFlyCompleted", OnSlideOrFlyCompleted), b.AddListener("CaptureCompleted", OnCaptureCompleted), b.AddListener("ShowMoveCompleted", OnShowMoveCompleted), b.Update(e.data.board, e.data.hand), lastHand = e.data.hand, SetState(e.data.state)
			} catch (a) {
				window.alert("Sorry your browser does not have the\ncapabilities required to run this program.\n\nThis program is optimized for the Chrome browser from Google.")
			}
			break;
		case "M_MakeMove_ACK":
			pendingState = e.data.state, b.ShowMove(e.data.move, e.data.board, e.data.hand), lastHand = e.data.hand;
			var o = document.getElementById("knodes");
			o.textContent = "A.I. Speed: " + e.data.knodes + "K nodes per second.";
			break;
		case "M_PlayUserMove_ACK":
			b.Update(e.data.board, e.data.hand), lastHand = e.data.hand, e.data.state == WHITE_MOVE || e.data.state == BLACK_MOVE ? (worker.postMessage({
				name: "M_MakeMove",
				thinkingTime: GetThinkingTime()
			}), SetState(THINKING)) : SetState(e.data.state);
			break;
		case "M_TakeBack_ACK":
			b.Update(e.data.board, e.data.hand), lastHand = e.data.hand, SetState(e.data.state);
			break;
		case "M_NewGame_ACK":
			b.Update(e.data.board, e.data.hand), lastHand = e.data.hand, SetState(e.data.state);
			break;
		case "M_StartUserMove_ACK":
			lastHand = e.data.hand, 0 == e.data.side ? e.data.hand.White > 0 ? b.GetInsert(e.data.side, e.data.inserts) : b.GetMoveStart(e.data.side, e.data.origins) : e.data.hand.Black > 0 ? b.GetInsert(e.data.side, e.data.inserts) : b.GetMoveStart(e.data.side, e.data.origins);
			break;
		case "M_GetCaptureLocations_ACK":
			0 == e.data.captures.length ? worker.postMessage({
				name: "M_PlayUserMove",
				fromRow: __fromRow,
				fromColumn: __fromColumn,
				toRow: __toRow,
				toColumn: __toColumn,
				captureRow: -1,
				captureColumn: -1
			}) : b.GetCapture(e.data.side, e.data.captures);
			break;
		case "M_GetMoveDestinations_ACK":
			b.SetDestinations(e.data.side, e.data.destinations)
		}
	}
	function GetThinkingTime() {
		var e = 1;
		return 1 == document.getElementById("one").checked && (e = 1), 1 == document.getElementById("five").checked && (e = 5), 1 == document.getElementById("fifteen").checked && (e = 15), 1 == document.getElementById("thirty").checked && (e = 30), e
	}
	var mm = mm || {},
		UNKNOWN = -1,
		WHITE_WIN = 0,
		BLACK_WIN = 1,
		WHITE_MOVE = 2,
		BLACK_MOVE = 3,
		DRAW = 4,
		THINKING = 5,
		worker, b, state, pendingState, lastHand, __fromRow, __fromColumn, __toRow, __toColumn, __captureRow, __captureColumn;