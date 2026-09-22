
const OPENXBL_BASE = "https://api.xbl.io";

export default {
  async fetch(request, env) {
    const url = new URL(request.url);

    if (request.method !== "GET" || url.pathname !== "/profile") {
      return json({ error: "not_found" }, 404);
    }

    const gamertag = (url.searchParams.get("gamertag") || "").trim();

    if (!gamertag || gamertag.length > 32) {
      return json({ error: "invalid_gamertag" }, 400);
    }

    if (!env.OPENXBL_API_KEY) {
      return json({ error: "server_not_configured" }, 500);
    }

    const upstream = await fetch(
      OPENXBL_BASE + "/v2/player/gamertag/" + encodeURIComponent(gamertag),
      {
        headers: {
          "X-Authorization": env.OPENXBL_API_KEY,
          "Accept": "application/json"
        }
      }
    );

    if (!upstream.ok) {
      return json(
        { error: "openxbl_error", status: upstream.status },
        upstream.status === 429 ? 429 : 502
      );
    }

    const data = await upstream.json();

    return json({
      gamertag: data.gamertag ?? null,
      gamerscore: data.gamerscore ?? null,
      gamerpic: data.profilePicture ?? null,
      xuid: data.xuid ?? null
    });
  }
};

function json(body, status = 200) {
  return new Response(JSON.stringify(body), {
    status,
    headers: {
      "content-type": "application/json; charset=utf-8",
      "cache-control": "no-store"
    }
  });
}
