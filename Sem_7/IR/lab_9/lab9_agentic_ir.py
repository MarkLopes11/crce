#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Lab 9 — Agentic AI for Information Retrieval (Ready-to-Run Template)
Author: (Your Name)
Usage:
  python lab9_agentic_ir.py --query "Impact of climate change on agriculture in India"
  python lab9_agentic_ir.py --query "What is RAG?" --max_steps 3

What this shows
- A lightweight "agent" that plans, acts (uses tools), and reflects to complete an IR task.
- Tools:
    1) LocalCorpusTool: builds/queries a FAISS index over /sample_corpus.
    2) WikipediaTool: does a quick search + summary via `wikipedia` package.
    3) LLMTool: summarizes/synthesizes evidence using a CPU-friendly model (FLAN-T5).
- Memory: simple JSON memory of past queries & sources (memory.json).

Install requirements (CPU-friendly):
    pip install wikipedia sentence-transformers faiss-cpu transformers torch --upgrade

If Wikipedia is blocked or offline, the agent will fall back to the local corpus.
"""

import os
import json
import argparse
import time
from dataclasses import dataclass, field
from typing import List, Dict, Any, Optional

# ---- Tool 0: Utilities
def truncate(text: str, n: int = 1200) -> str:
    return text if len(text) <= n else text[:n] + "...[truncated]"

# ---- Tool 1: Local Corpus Retrieval (FAISS + sentence-transformers)
class LocalCorpusTool:
    def __init__(self, docs_dir: str, index_path: str, model_name: str = "sentence-transformers/all-MiniLM-L6-v2"):
        self.docs_dir = docs_dir
        self.index_path = index_path
        self.model_name = model_name
        self.emb = None
        self.faiss = None
        self.doc_texts = []
        self.doc_names = []

        if not os.path.isdir(docs_dir):
            os.makedirs(docs_dir, exist_ok=True)

    def _lazy_import(self):
        global faiss, SentenceTransformer
        import faiss  # type: ignore
        from sentence_transformers import SentenceTransformer  # type: ignore
        self.faiss = faiss
        if self.emb is None:
            self.emb = SentenceTransformer(self.model_name)

    def _load_docs(self):
        self.doc_texts, self.doc_names = [], []
        for fn in os.listdir(self.docs_dir):
            if fn.lower().endswith((".txt", ".md")):
                path = os.path.join(self.docs_dir, fn)
                with open(path, "r", encoding="utf-8") as f:
                    txt = f.read().strip()
                self.doc_texts.append(txt)
                self.doc_names.append(fn)

    def build_or_load(self):
        self._lazy_import()
        self._load_docs()
        vectors = self.emb.encode(self.doc_texts, show_progress_bar=False)
        d = vectors.shape[1]
        index = self.faiss.IndexFlatIP(d)  # cosine via normalized dot
        # normalize
        import numpy as np
        norms = np.linalg.norm(vectors, axis=1, keepdims=True) + 1e-10
        vectors = vectors / norms
        index.add(vectors.astype("float32"))
        self.index = index

    def search(self, query: str, k: int = 3) -> List[Dict[str, str]]:
        self._lazy_import()
        if not hasattr(self, "index"):
            self.build_or_load()
        qv = self.emb.encode([query], show_progress_bar=False)
        import numpy as np
        qv = qv / (np.linalg.norm(qv, axis=1, keepdims=True) + 1e-10)
        D, I = self.index.search(qv.astype("float32"), k)
        results = []
        for idx in I[0]:
            if idx < 0 or idx >= len(self.doc_texts):
                continue
            results.append({"source": f"local:{self.doc_names[idx]}", "text": self.doc_texts[idx]})
        return results

# ---- Tool 2: Wikipedia Search (online)
class WikipediaTool:
    def __init__(self, lang: str = "en", max_chars: int = 1200):
        self.lang = lang
        self.max_chars = max_chars

    def _lazy_import(self):
        global wikipedia
        import wikipedia  # type: ignore
        wikipedia.set_lang(self.lang)

    def search_and_summarize(self, query: str, n: int = 3) -> List[Dict[str, str]]:
        try:
            self._lazy_import()
            titles = wikipedia.search(query)[:n]
            results = []
            for t in titles:
                try:
                    page = wikipedia.page(t, auto_suggest=False)
                    summ = wikipedia.summary(t, sentences=6)
                    results.append({
                        "source": f"wikipedia:{page.title}",
                        "url": page.url,
                        "text": summ[: self.max_chars]
                    })
                except Exception:
                    continue
            return results
        except Exception:
            # Offline or blocked; return empty
            return []

# ---- Tool 3: LLM for synthesis (FLAN-T5 on CPU)
class LLMTool:
    def __init__(self, model_name: str = "google/flan-t5-base", max_new_tokens: int = 256):
        self.model_name = model_name
        self.max_new_tokens = max_new_tokens
        self.pipe = None

    def _lazy_import(self):
        if self.pipe is None:
            from transformers import pipeline  # type: ignore
            self.pipe = pipeline("text2text-generation", model=self.model_name)

    def answer(self, query: str, evidence: List[Dict[str, str]]) -> str:
        self._lazy_import()
        # Prepare a compact prompt with citations
        chunks = []
        for i, ev in enumerate(evidence, 1):
            src = ev.get("source", "unknown")
            txt = ev.get("text", "")
            chunks.append(f"[{i}] {src} :: {txt}")
        context = "\n".join(chunks)
        prompt = (
            "You are an IR agent. Synthesize a concise answer (120-200 words) using the evidence below. "
            "Cite sources in-line like [1], [2]. If evidence is insufficient, say what is missing.\n\n"
            f"Question: {query}\n\nEvidence:\n{context}\n\nAnswer:"
        )
        out = self.pipe(prompt, max_new_tokens=self.max_new_tokens)[0]["generated_text"]
        return out.strip()

# ---- Agent
@dataclass
class AgentConfig:
    max_steps: int = 3
    use_wikipedia: bool = True
    use_local: bool = True

@dataclass
class AgentState:
    query: str
    steps: List[Dict[str, Any]] = field(default_factory=list)
    evidence: List[Dict[str, str]] = field(default_factory=list)
    final_answer: Optional[str] = None

class AgenticIRAgent:
    def __init__(self, docs_dir: str, memory_path: str):
        self.local_tool = LocalCorpusTool(docs_dir, index_path=os.path.join(docs_dir, "faiss.index"))
        self.wiki_tool = WikipediaTool()
        self.llm_tool = LLMTool()
        self.memory_path = memory_path
        self.memory = self._load_memory()

    # --- Simple memory (past queries & sources)
    def _load_memory(self) -> Dict[str, Any]:
        if os.path.exists(self.memory_path):
            try:
                return json.load(open(self.memory_path, "r", encoding="utf-8"))
            except Exception:
                return {}
        return {}

    def _save_memory(self):
        try:
            json.dump(self.memory, open(self.memory_path, "w", encoding="utf-8"), indent=2, ensure_ascii=False)
        except Exception:
            pass

    def plan(self, state: AgentState) -> str:
        # Rule-based plan
        if not state.evidence and self.memory.get("last_sources") == "local":
            return "TRY_WIKIPEDIA"
        if not state.evidence and self.memory.get("last_sources") == "wikipedia":
            return "TRY_LOCAL"
        # Default: try local first, then wiki
        if not state.evidence:
            return "TRY_LOCAL"
        # Have some evidence; proceed to synthesize
        return "SYNTHESIZE"

    def act(self, action: str, state: AgentState) -> AgentState:
        if action == "TRY_LOCAL":
            docs = self.local_tool.search(state.query, k=3)
            state.steps.append({"action": action, "num_docs": len(docs)})
            state.evidence.extend(docs)
            self.memory["last_sources"] = "local"
        elif action == "TRY_WIKIPEDIA":
            docs = self.wiki_tool.search_and_summarize(state.query, n=3)
            state.steps.append({"action": action, "num_docs": len(docs)})
            state.evidence.extend(docs)
            self.memory["last_sources"] = "wikipedia"
        elif action == "SYNTHESIZE":
            answer = self.llm_tool.answer(state.query, state.evidence)
            state.steps.append({"action": action, "used_docs": len(state.evidence)})
            state.final_answer = answer
        else:
            state.steps.append({"action": "NOOP"})
        return state

    def reflect(self, state: AgentState) -> bool:
        """Return True if we should continue iterating."""
        if state.final_answer:
            return False
        # If we have no evidence yet, continue
        if not state.evidence:
            return True
        # If evidence is from only one source type and steps left, try the other
        sources = [e.get("source","") for e in state.evidence]
        has_local = any(s.startswith("local:") for s in sources)
        has_wiki = any(s.startswith("wikipedia:") for s in sources)
        if not (has_local and has_wiki) and len(state.steps) < 2:
            return True
        # Otherwise, synthesize next
        return True

    def run(self, query: str, cfg: AgentConfig) -> AgentState:
        state = AgentState(query=query)
        for step in range(cfg.max_steps):
            action = self.plan(state)
            state = self.act(action, state)
            if not self.reflect(state):
                break
        # Finalize: if still no answer, try to synthesize anyway
        if state.final_answer is None and state.evidence:
            state = self.act("SYNTHESIZE", state)
        self._save_memory()
        return state

# ---- CLI
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--query", type=str, required=True, help="User question")
    parser.add_argument("--docs_dir", type=str, default=os.path.join(os.path.dirname(__file__), "sample_corpus"))
    parser.add_argument("--memory_path", type=str, default=os.path.join(os.path.dirname(__file__), "memory.json"))
    parser.add_argument("--max_steps", type=int, default=3)
    args = parser.parse_args()

    agent = AgenticIRAgent(docs_dir=args.docs_dir, memory_path=args.memory_path)
    cfg = AgentConfig(max_steps=args.max_steps)
    t0 = time.time()
    state = agent.run(args.query, cfg)
    dt = time.time() - t0

    print("="*80)
    print(f"Query: {args.query}")
    print("- Steps -")
    for s in state.steps:
        print(s)
    print("- Evidence -")
    for i, ev in enumerate(state.evidence, 1):
        src = ev.get("source","")
        url = ev.get("url","")
        print(f"[{i}] {src} {('('+url+')') if url else ''}")
    print("\n- Final Answer -")
    print(state.final_answer or "(No answer generated)")
    print("-"*80)
    print(f"Completed in {dt:.2f}s")
    print("="*80)

if __name__ == "__main__":
    main()
