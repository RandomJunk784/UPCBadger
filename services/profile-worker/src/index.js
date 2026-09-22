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

    const cacheKey = new Request(
      new URL("/_cache/profile/" + encodeURIComponent(gamertag.toLowerCase()), request.url),
      { method: "GET" }
    );

    const cache = caches.default;
    const cached = await cache.match(cacheKey);

    if (cached) {
      const response = new Response(cached.body, cached);
      response.headers.set("X-UPCBadger-Cache", "HIT");
      return response;
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

    const response = json({
      gamertag: data.gamertag ?? null,
      gamerscore: data.gamerscore ?? null,
      gamerpic: data.profilePicture ?? null,
      xuid: data.xuid ?? null
    });

    response.headers.set("Cache-Control", "public, max-age=900");
    response.headers.set("X-UPCBadger-Cache", "MISS");
    await cache.put(cacheKey, response.clone());
    return response;
  }
};

function json(body, status = 200) {
  return new Response(JSON.stringify(body), {
    status,
    headers: {
      "content-type": "application/json; charset=utf-8",
      "cache-control": "no-store",
      "access-control-allow-origin": "*"
    }
  });
}