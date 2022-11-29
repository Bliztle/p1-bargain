const dos = async () => {

    const response = await fetch("https://api.cl.coop.dk/storeapi/v1/stores?page=1&size=1000", {
        headers: {
            "Ocp-Apim-Subscription-Key": "8042a78a1c91463e80140b0cb11b8b47"
        }
    });

    console.log(response);

    const json = await response.json();

    console.log(json);

    const retailGroups = new Set();

    console.log(retailGroups);

    json["Data"].forEach(d => retailGroups.add(d["RetailGroupName"]));

    console.log(retailGroups);
    retailGroups.forEach(d => console.log(d));

};

dos();