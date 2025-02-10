// Set up the dimensions and grid
const width = 800;
const height = 600;
const numNodes = 20; // Smaller number for demonstration
const numCols = 5;  // Columns in grid for layout

// Node types with different colors for demonstration
const placeTypes = ["Red Node", "Blue Node"];

// Nodes and links with directional arrows
const nodes = Array.from({ length: numNodes }, (_, i) => ({
    id: i,
    x: (i % numCols) * (width / numCols) + (width / numCols) / 2,
    y: Math.floor(i / numCols) * (height / (numNodes / numCols)) + (height / (numNodes / numCols)) / 2,
    color: i % 2 === 0 ? "red" : "blue"
}));

const links = [];
nodes.forEach((node, i) => {
    if ((i + 1) % numCols !== 0) {
        links.push({ source: i, target: i + 1, bidirectional: i % 2 === 0 });
    }
    if (i + numCols < numNodes) {
        links.push({ source: i, target: i + numCols, bidirectional: i % 2 !== 0 });
    }
});

// Set up the SVG canvas
const svg = d3.select("#map").append("svg")
    .attr("width", width)
    .attr("height", height);

// Add arrow markers for one-way and bidirectional edges
svg.append("defs").selectAll("marker")
    .data(["end", "end-both"])
    .enter().append("marker")
    .attr("id", d => d)
    .attr("viewBox", "0 -5 10 10")
    .attr("refX", 15)
    .attr("refY", 0)
    .attr("markerWidth", 6)
    .attr("markerHeight", 6)
    .attr("orient", "auto")
    .append("path")
    .attr("d", "M0,-5L10,0L0,5")
    .attr("fill", d => d === "end" ? "#555" : "#555");

// Draw links (edges) with markers for directions
const link = svg.append("g")
    .attr("class", "links")
    .selectAll("line")
    .data(links)
    .enter().append("line")
    .attr("class", "link")
    .attr("stroke-width", 3)
    .attr("stroke", d => d.bidirectional ? "#1f77b4" : "#d62728")
    .attr("marker-end", d => d.bidirectional ? "url(#end-both)" : "url(#end)");

// Draw nodes with colors and interactive labels
const node = svg.append("g")
    .attr("class", "nodes")
    .selectAll("g")
    .data(nodes)
    .enter().append("g")
    .attr("class", "node")
    .attr("transform", d => `translate(${d.x},${d.y})`)
    .call(d3.drag()
        .on("start", dragstarted)
        .on("drag", dragged)
        .on("end", dragended));

// Add circles with different colors for nodes
node.append("circle")
    .attr("r", 8)
    .attr("fill", d => d.color);

// Tooltips for each node
node.append("title")
    .text((d, i) => `${placeTypes[i % 2]} ${Math.floor(i / placeTypes.length) + 1}`);

// Update positions each tick of the simulation
d3.forceSimulation(nodes)
    .force("link", d3.forceLink(links).distance(60).strength(1).iterations(10))
    .force("charge", d3.forceManyBody().strength(-30))
    .force("center", d3.forceCenter(width / 2, height / 2))
    .on("tick", () => {
        link.attr("x1", d => nodes[d.source].x)
            .attr("y1", d => nodes[d.source].y)
            .attr("x2", d => nodes[d.target].x)
            .attr("y2", d => nodes[d.target].y);

        node.attr("transform", d => `translate(${d.x},${d.y})`);
    });

// Dragging functions to move nodes
function dragstarted(event, d) {
    if (!event.active) d3.forceSimulation().alphaTarget(0.3).restart();
    d.fx = d.x;
    d.fy = d.y;
}

function dragged(event, d) {
    d.fx = event.x;
    d.fy = event.y;
}

function dragended(event, d) {
    if (!event.active) d3.forceSimulation().alphaTarget(0);
    d.fx = null;
    d.fy = null;
}
